#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 1000

// 状态结构
typedef struct {
    int ML, CL; // 左岸修道士和野人数量
    int MR, CR; // 右岸修道士和野人数量
    int B;      // 船的位置，0 表示左岸，1 表示右岸
} State;

// 队列节点结构
typedef struct Node {
    State state;        // 当前状态
    struct Node* parent; // 父节点，记录路径
} Node;

// 访问标记数组
int visited[MAX_STATES][MAX_STATES][2] = {0}; 

// 检测状态是否安全
int is_safe(State s) {
    if (s.ML > 0 && s.ML < s.CL) return 0; // 左岸不安全
    if (s.MR > 0 && s.MR < s.CR) return 0; // 右岸不安全
    return 1;
}

// 添加一个新状态
void add_state(State* next_states, int* count, int ML, int CL, int MR, int CR, int B, int N) {
    State new_state = {ML, CL, MR, CR, B};
    if (ML >= 0 && CL >= 0 && MR >= 0 && CR >= 0 && ML <= N && CL <= N && MR <= N && CR <= N && is_safe(new_state)) {
        next_states[(*count)++] = new_state;
    }
}

// 生成所有可能的下一个状态
int generate_next_states(State current, State* next_states, int C, int N) {
    int count = 0;
    int ML = current.ML, CL = current.CL, MR = current.MR, CR = current.CR, B = current.B;

    if (B == 0) { // 船在左岸
        for (int m = 0; m <= C; m++) {
            for (int c = 0; c <= C - m; c++) {
                if (m + c == 0 || m + c > C) continue; // 无效组合
                add_state(next_states, &count, ML - m, CL - c, MR + m, CR + c, 1, N);
            }
        }
    } else { // 船在右岸
        for (int m = 0; m <= C; m++) {
            for (int c = 0; c <= C - m; c++) {
                if (m + c == 0 || m + c > C) continue; // 无效组合
                add_state(next_states, &count, ML + m, CL + c, MR - m, CR - c, 0, N);
            }
        }
    }

    return count;
}

// 打印路径
void print_path(Node* node) {
    Node* path[MAX_STATES];
    int length = 0;

    // 回溯路径
    while (node != NULL) {
        path[length++] = node;
        node = node->parent;
    }

    // 反向输出路径
    printf("安全过河路径：\n");
    for (int i = length - 1; i >= 0; i--) {
        State s = path[i]->state;
        printf("左岸: 修道士=%d, 野人=%d | 右岸: 修道士=%d, 野人=%d | 船位置: %s\n",
               s.ML, s.CL, s.MR, s.CR, s.B == 0 ? "左岸" : "右岸");
    }
}

// 广度优先搜索
void bfs(int N, int C) {
    Node* queue[MAX_STATES];
    int front = 0, rear = 0;

    // 初始状态
    State initial = {N, N, 0, 0, 0};
    Node* initial_node = (Node*)malloc(sizeof(Node));
    initial_node->state = initial;
    initial_node->parent = NULL;

    queue[rear++] = initial_node;
    visited[N][N][0] = 1;

    while (front < rear) {
        Node* current = queue[front++];
        State state = current->state;

        // 检查是否到达目标状态
        if (state.ML == 0 && state.CL == 0 && state.MR == N && state.CR == N && state.B == 1) {
            print_path(current);
            return;
        }

        // 生成下一步状态
        State next_states[MAX_STATES];
        int count = generate_next_states(state, next_states, C, N);

        for (int i = 0; i < count; i++) {
            State next = next_states[i];
            if (!visited[next.ML][next.CL][next.B]) {
                visited[next.ML][next.CL][next.B] = 1;

                Node* next_node = (Node*)malloc(sizeof(Node));
                next_node->state = next;
                next_node->parent = current;

                queue[rear++] = next_node;
            }
        }
    }

    printf("无解！\n");
}
int main(){
	int N,C;
	printf("请输入N:");
	scanf("%d",&N);
	printf("请输入C:");
	scanf("%d",&C); 
	bfs(N,C);
	return 0;
} 






