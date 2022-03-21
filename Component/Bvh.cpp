#include "Bvh.h"
#include <cstdio>
#include <algorithm>
#include <atomic>
#include <ctime>
static void safeFree(void* p)
{
    if (p) {
        std::free(p);
    }
}
static unsigned int expandBits(unsigned int v)
{
    v = (v * 0x00010001u) & 0xFF0000FFu;
    v = (v * 0x00000101u) & 0x0F00F00Fu;
    v = (v * 0x00000011u) & 0xC30C30C3u;
    v = (v * 0x00000005u) & 0x49249249u;
    return v;
}
static double my_min(double a, double b)
{
    return a < b ? a : b;
}
static double my_max(double a, double b)
{
    return a > b ? a : b;
}

unsigned int ZLib::getMortonCode(double x, double y, double z)
{
    x = my_min(my_max(x * 1024.0f, 0.0f), 1023.0f);
    y = my_min(my_max(y * 1024.0f, 0.0f), 1023.0f);
    z = my_min(my_max(z * 1024.0f, 0.0f), 1023.0f);
    unsigned int xx = expandBits((unsigned int)x);
    unsigned int yy = expandBits((unsigned int)y);
    unsigned int zz = expandBits((unsigned int)z);
    return xx * 4 + yy * 2 + zz;
}

static void sortLeafNodes(ZLib::LeafNodes& nodes, int n)
{
    struct Cmp
    {
    public:
        Cmp(unsigned int* codes_):codes(codes_)
        {

        }
        bool operator()(const int& a, const int& b)const
        {
            return codes[a] < codes[b];
        }
        unsigned int* codes;
    };
    auto codes = nodes.morton_codes;
    auto ids = nodes.data_ids;
    std::sort(ids, ids + n, Cmp(codes));
    unsigned int* tmp = (unsigned int*)malloc(n * sizeof(unsigned int));
    for (int i = 0; i < n; ++i) {
        tmp[i] = codes[ids[i]];
    }
    nodes.morton_codes = tmp;
    safeFree(codes);
}
// 给叶节点赋值，以及给叶节点所使用的aabb赋值
void initLeafNodes(ZLib::LeafNodes leaf_nodes, AABB* aabbs, const std::vector<IShape*>& shapes)
{
    int n =shapes.size();
    float min_s = INFINITY;
    float max_s = -INFINITY;
    float center_p[3];
    for (int i = 0; i < n; ++i) {
        auto& aabb = shapes[i]->getBoundingBox();
        for (int j = 0; j < 3; ++j) {
            min_s = my_min(min_s, aabb.min_value[j]);
            max_s = my_max(max_s, aabb.max_value[j]);
        }
    }
    for (int offset = 0; offset < n; ++offset) {
        size_t aabb_offset = n - 1 + offset;
        leaf_nodes.data_ids[offset] = offset;

        auto& aabb = shapes[offset]->getBoundingBox();
        aabbs[aabb_offset] = aabb;

        center_p[0] = (aabb.min_value.x() + aabb.max_value.x()) / 2.0f;
        center_p[1] = (aabb.min_value.y() + aabb.max_value.y()) / 2.0f;
        center_p[2] = (aabb.min_value.z() + aabb.max_value.z()) / 2.0f;
        for (int i = 0; i < 3; ++i) {
            center_p[i] -= min_s;
            center_p[i] /= (max_s - min_s);
        }
        leaf_nodes.morton_codes[offset] = ZLib::getMortonCode(center_p[0], center_p[1], center_p[2]);
    }
}

static int clz(unsigned int a)
{
    int c = 0;
    for (int i = 31; i >= 0; --i) {
        if (a & (1 << i)) {
            break;
        }
        ++c;
    }
    return c;
}

static int getSimilarity(const unsigned int* morton_codes, int n, int i, int j)
{
    if (i < 0 || i >= n || j < 0 || j >= n) {
        return -1;
    }
    unsigned int a = morton_codes[i];
    unsigned int b = morton_codes[j];
    int k = 0;
    if (a == b) {
        k = clz(i ^ j);
    }
    return clz(a ^ b) + k;
}
static int sign(int a)
{
    return a > 0 ? 1 : -1;
}

static void buildTree(ZLib::BVHNodes internal_nodes, ZLib::LeafNodes leaf_nodes, int triangle_num)
{
    for (int i = 0; i < triangle_num - 1;++i) {
        unsigned int* morton_codes = leaf_nodes.morton_codes;
        int d = sign(getSimilarity(morton_codes, triangle_num, i, i + 1) - getSimilarity(morton_codes, triangle_num, i, i - 1));
        int sigma_min = getSimilarity(morton_codes, triangle_num, i, i - d);
        int l_max = 2;
        while (getSimilarity(morton_codes, triangle_num, i, i + l_max * d) > sigma_min) {
            l_max *= 2;
        }
        int l = 0;
        for (int t = l_max / 2; t >= 1; t /= 2) {
            if (getSimilarity(morton_codes, triangle_num, i, i + (l + t) * d) > sigma_min) {
                l += t;
            }
        }
        int j = i + l * d;
        int sigma_node = getSimilarity(morton_codes, triangle_num, i, j);
        /*int div = 2;
        int t = (l + div - 1) / div;
        int s = 0;
        while (t >= 1) {
            div *= 2;
            t = (l + div - 1) / div;
            if (getSimilarity(morton_codes, n, i, i + (s + t) * d) > sigma_node) {
                s += t;
            }
        }*/
        int left = my_min(i, j);
        int judge_p = left;
        int right = my_max(i, j);
        int mid = 0;

        while (left <= right) {
            mid = (left + right) / 2;
            if (getSimilarity(morton_codes, triangle_num, judge_p, mid) > sigma_node) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        int gamma = right;
        internal_nodes.lefts[i] = gamma;
        internal_nodes.rights[i] = gamma + 1;
        if (my_min(i, j) == gamma) {
            internal_nodes.l_types[i] = 0;
            leaf_nodes.fa[gamma] = i;
        } else {
            internal_nodes.l_types[i] = 1;
            internal_nodes.fa[gamma] = i;
        }
        if (my_max(i, j) == gamma + 1) {
            internal_nodes.r_types[i] = 0;
            leaf_nodes.fa[gamma + 1] = i;
        } else {
            internal_nodes.r_types[i] = 1;
            internal_nodes.fa[gamma + 1] = i;

        }
    }
}

static void mergeAABB(int result, int a, int b, AABB* aabbs)
{
    aabbs[result].min_value = ZLib::min(aabbs[a].min_value, aabbs[b].min_value);
    aabbs[result].max_value = ZLib::max(aabbs[a].max_value, aabbs[b].max_value);
}

//void printfAABB(ZLib::AABBs aabbs, int a)
//{
//    printf("AABB %d x0:%lf, x1:%lf, y0:%lf, y1:%lf z0:%lf, z1:%lf\n", a, aabbs.x0[a], aabbs.x1[a], aabbs.y0[a], aabbs.y1[a], aabbs.z0[a], aabbs.z1[a]);
//}
//void printfPoint(ZLib::PointsSet p, int a)
//{
//    printf("x:%lf y:%lf z:%lf \n", p.x[a], p.y[a], p.z[a]);
//}
//void printfTri(ZLib::PointsSet p, ZLib::TrianglesSet t, int a)
//{
//    printf("Triangle %d\n", a);
//    for (int i = 0; i < 3; ++i) {
//        printfPoint(p, t.v[i][a]);
//    }
//}
static void mergeAABBs(int* visit, ZLib::BVHNodes internal_nodes, ZLib::LeafNodes leaf_nodes, AABB* aabbs, int n)
{

    for (int leaf_id = 0; leaf_id < n;++leaf_id) {
        int fa = leaf_nodes.fa[leaf_id];
        while (fa != -1) {
            int add_result = visit[fa];
            ++visit[fa];
            if (add_result == 1) {
                int a = internal_nodes.lefts[fa];
                int b = internal_nodes.rights[fa];
                if (internal_nodes.l_types[fa] == 0) {
                    a = leaf_nodes.data_ids[a];
                    a += n - 1;

                }
                if (internal_nodes.r_types[fa] == 0) {
                    b = leaf_nodes.data_ids[b];
                    b += n - 1;

                }
                mergeAABB(fa, a, b, aabbs);

                fa = internal_nodes.fa[fa];

            } else {
                break;
            }
        }
    }
}




ZLib::BVH::BVH():aabbs(nullptr),internal_nodes({nullptr,nullptr,nullptr,nullptr,nullptr}), leaf_nodes({nullptr,nullptr,nullptr}),stacks1(nullptr),stacks2(nullptr),triangle_num(0)
{
    
}

void ZLib::BVH::build(const std::vector<IShape*>&shapes, int thread_num)
{
    this->shapes = shapes;
    triangle_num = shapes.size();
    float t;
    clock_t  start = std::clock();
    allocDeviceMemory(shapes, thread_num);
    clock_t end = std::clock();
    t = (float)(end - start) / (float)CLOCKS_PER_SEC;
    printf("\tAllocate memory: %lfms\n", t * 1000);

    start = std::clock();
    initLeafNodes(leaf_nodes, aabbs, shapes);
    end = std::clock();
    t = (float)(end - start) / (float)CLOCKS_PER_SEC;
    printf("\tInit leaf node: %lfms\n", t * 1000);

    start = std::clock();
    sortLeafNodes(leaf_nodes, triangle_num);
    end = std::clock();
    t = (float)(end - start) / (float)CLOCKS_PER_SEC;
    printf("\tSort leaf node by morton code: %lfms\n", t * 1000);

    start = std::clock();
    buildTree(internal_nodes, leaf_nodes, triangle_num);
    end = std::clock();
    t = (float)(end - start) / (float)CLOCKS_PER_SEC;
    printf("\tBuild BVH tree: %lfms\n", t * 1000);


    start = std::clock();
    int* visits = nullptr;
    visits = (int*)malloc(triangle_num * sizeof(*visits));
    std::memset(visits, 0, triangle_num * sizeof(*visits));
    mergeAABBs(visits, internal_nodes, leaf_nodes, aabbs, triangle_num);
    safeFree(visits);
    end = std::clock();
    t = (float)(end - start) / (float)CLOCKS_PER_SEC;
    printf("\tMerge AABBs: %lfms\n", t * 1000);
}



int ZLib::BVH::getTriangleNum() const
{
    return triangle_num;
}


static bool checkInBox(int tri_id, int aabb_id, const AABB* const aabbs)
{

}

void my_swap(int& a, int& b)
{
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}

Intersection ZLib::BVH::find(int root, const Ray& ray, int thread_id)
{
    int n = triangle_num - 1;
   /* cs[offset]++;

    if (internal_nodes.l_types[root] == 0) {
        int id1 = internal_nodes.lefts[root];
        int id2 = leaf_nodes.data_ids[id1];
        if (offset < id1 && checkIntersection(triangle_id, id2, points, triangles)) {
            int l = triangle_id < id2 ? triangle_id : id2;
            int r = triangle_id < id2 ? id2 : triangle_id;
            printf("contact found at (%d, %d)\n", l, r);
        }
    } else {
        int id2 = internal_nodes.lefts[root];
        if (offset < id2 && checkInBox(triangle_id + n, id2, aabbs)) {
            find(id2, triangle_id, offset);
        }
    }

    if (internal_nodes.r_types[root] == 0) {
        int id1 = internal_nodes.rights[root];
        int id2 = leaf_nodes.data_ids[id1];
        if (offset < id1 && checkIntersection(triangle_id, id2, points, triangles)) {
            int l = triangle_id < id2 ? triangle_id : id2;
            int r = triangle_id < id2 ? id2 : triangle_id;
            printf("contact found at (%d, %d)\n", l, r);
        }
    } else {
        int id2 = internal_nodes.rights[root];
        if (checkInBox(triangle_id + n, id2, aabbs)) {
            find(id2, triangle_id, offset);
        }
    }*/
#define STACK_SIZE 32
    int top = -1;
    int max_l = STACK_SIZE;
    int* stack1 = stacks1 + STACK_SIZE * thread_id;
    int* stack2 = stacks2 + STACK_SIZE * thread_id;
    ++top;
    stack1[top] = root;
    stack2[top] = 0;
    bool first = true;
    Intersection ret;
    ret.t = INFINITY;
    while (top >= 0) {
        int node = stack1[top];
        int visit_type = stack2[top];
        bool node_type;
        int child;
        bool flag = true;
        if (visit_type == 0) {
            node_type = internal_nodes.l_types[node];
            child = internal_nodes.lefts[node];
            stack2[top] = 1;
            //flag = (offset < child);
        } else if (visit_type == 1) {
            node_type = internal_nodes.r_types[node];
            child = internal_nodes.rights[node];
            stack2[top] = 2;
        } else {
            --top;
            continue;
        }
        if (node_type == 0) {
            int id2 = leaf_nodes.data_ids[child];
            Intersection intersection;
            auto is_intersect = shapes[id2]->intersect(ray, intersection);
            if (is_intersect && intersection.t < ret.t) {
                ret = intersection;
            }
        } else {
            if (aabbs[child].isIntersect(ray)) {
                ++top;
                if (top == max_l) {
                    int* tmp = (int*)malloc(max_l * 2 * sizeof(int));
                    int* tmp2 = (int*)malloc(max_l * 2 * sizeof(int));
                    for (int i = 0; i < top; ++i) {
                        tmp[i] = stack1[i];
                    }
                    for (int i = 0; i < top; ++i) {
                        tmp2[i] = stack2[i];
                    }
                    if (!first) {
                        safeFree(stack1);
                        safeFree(stack2);
                    }
                    first = false;
                    stack1 = tmp;
                    stack2 = tmp2;
                    max_l *= 2;
                }
                stack1[top] = child;
                stack2[top] = 0;
            }
        }
    }
    if (!first) {
        safeFree(stack1);
        safeFree(stack2);
    }
    return ret;
}

void ZLib::BVH::free()
{
    safeFree(internal_nodes.lefts);
    safeFree(internal_nodes.l_types);
    safeFree(internal_nodes.rights);
    safeFree(internal_nodes.r_types);
    safeFree(internal_nodes.fa);

    safeFree(leaf_nodes.data_ids);
    safeFree(leaf_nodes.morton_codes);
    safeFree(leaf_nodes.fa);

    safeFree(aabbs);


    safeFree(stacks1);
    safeFree(stacks2);
}

ZLib::BVH::~BVH()
{
    free();
}

void ZLib::BVH::allocDeviceMemory(const std::vector<IShape*>& shapes, int thread_num)
{
    stacks1 = (int*)malloc(sizeof(int) * STACK_SIZE * thread_num);
    stacks2 = (int*)malloc(sizeof(int) * STACK_SIZE * thread_num);
    leaf_nodes.data_ids = (int*)std::malloc(triangle_num * sizeof(*(leaf_nodes.data_ids)));
    leaf_nodes.morton_codes = (unsigned int*)std::malloc(triangle_num * sizeof(*(leaf_nodes.morton_codes)));
    leaf_nodes.fa = (int*)std::malloc(triangle_num * sizeof(*(leaf_nodes.fa)));

    // 分配非叶节点，数量为三角形数-1
    size_t n = triangle_num - 1;
    internal_nodes.lefts = (int*)std::malloc(n * sizeof(*(internal_nodes.lefts)));
    internal_nodes.l_types = (bool*)std::malloc(n * sizeof(*(internal_nodes.l_types)));
    internal_nodes.rights = (int*)std::malloc(n * sizeof(*(internal_nodes.rights)));
    internal_nodes.r_types = (bool*)std::malloc(n * sizeof(*(internal_nodes.r_types)));
    internal_nodes.fa = (int*)std::malloc(n * sizeof(*(internal_nodes.fa)));

    // 设根节点的父节点为-1
    std::memset(internal_nodes.fa, -1, sizeof(*(internal_nodes.fa)));

    // 分配AABB内存，数量为2*triangle_num-1
    size_t m = 2 * triangle_num - 1;
    aabbs = (AABB*)malloc(m * sizeof(AABB));
}



