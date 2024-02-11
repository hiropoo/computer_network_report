#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// ダイクストラ砲で計算した結果を保存する構造体
typedef struct {
    int node;
    int cost;
    int parent;
    int isFixed;
} NODEINFO;

/* プロトタイプ宣言 */
// コストテーブルを作成する関数
void makeCostTable(int nodeNum, int costTable[nodeNum][nodeNum], int pathNum, FILE *fp);

// リザルトテーブルを作成する関数
void makeResultTable(int nodeNum, NODEINFO resultTable[nodeNum], int startNode);

// ダイクストラ法を実行する関数
void ExeDijkstra(int nodeNum, int costTable[nodeNum][nodeNum], int startNode, int endNode, NODEINFO resultTable[nodeNum]);

// リザルトテーブルを表示する関数
void printResultTable(int nodeNum, NODEINFO resultTable[nodeNum]);
// ----- プロトタイプ宣言ここまで -----


int main(void) {
    // ファイルの読み込み
    char fileName[] = "example_NodePath.txt";  // ファイル名
    FILE *fp = NULL;                           // ファイルポインタ
    if ((fp = fopen(fileName, "r")) == NULL) {
        printf("%s : ファイルを開けませんでした\n", fileName);
        exit(-1);
    }


    // パスの数とノードの数を入力
    int pathNum = -1;  // パスの数
    int nodeNum = -1;  // ノードの数
    fscanf(fp, "%d %d", &pathNum, &nodeNum);


    // スタートノードとゴールノードを入力
    int startNode = -1;  // スタートノード
    int endNode = -1;    // ゴールノード
    do {
        printf("スタートノードとゴールノードを入力してください\n");
        printf("スタート: ");
        scanf("%d", &startNode);
        printf("ゴール: ");
        scanf("%d", &endNode);
    } while (startNode < 0 || startNode >= nodeNum || endNode < 0 || endNode >= nodeNum);


    // コストテーブル（経路表）を作成
    int costTable[nodeNum][nodeNum];  // コストテーブル（経路表）
    makeCostTable(nodeNum, costTable, pathNum, fp);


    // ダイクストラ法の結果を保存するリザルトテーブルを作成
    NODEINFO resultTable[nodeNum];
    makeResultTable(nodeNum, resultTable, startNode);


    // ダイクストラ法を実行しリザルトテーブルを作成
    ExeDijkstra(nodeNum, costTable, startNode, endNode, resultTable);


    // リザルトテーブルの表示
    printResultTable(nodeNum, resultTable);

    return 0;
}


// コストテーブル（経路表）を作成する関数
void makeCostTable(int nodeNum, int costTable[nodeNum][nodeNum], int pathNum,
                   FILE *fp) {
    // コストテーブルの初期化
    for (int i = 0; i < nodeNum; i++)
        for (int j = 0; j < nodeNum; j++) costTable[i][j] = -1;

    // コストテーブル（経路表）の作成
    for (int i = 0; i < pathNum; i++) {
        // 1行（出発点 到着点 かかるコスト）ごと読み取り
        int src = -1, dst = -1, cost = -1;
        fscanf(fp, "%d %d %d", &src, &dst, &cost);

        // 表にコストを記入。出発点と到着点が逆の場合も記入する
        costTable[src][dst] = cost;
        costTable[dst][src] = cost;
    }
}


// リザルトテーブルを作成する関数
void makeResultTable(int nodeNum, NODEINFO resultTable[nodeNum], int startNode) {
    for (int i = 0; i < nodeNum; i++) {
        resultTable[i].node = i;
        resultTable[i].cost = INT_MAX;  // 最初のコストは無限大
        resultTable[i].isFixed = 0;

        if (i == startNode) {                   // スタートノードの場合
            resultTable[i].parent = startNode;  // 親は自分自身
            resultTable[i].cost = 0;            // スタートノードのコストは0
            resultTable[i].isFixed = 1;         // スタートノードは確定
        } else {
            resultTable[i].parent = -1;
        }
    }
}


// ダイクストラ法を実行する関数
void ExeDijkstra(int nodeNum, int costTable[nodeNum][nodeNum], int startNode, int endNode, NODEINFO resultTable[nodeNum]) {
    int src = startNode;    // 出発ノード
    do {
        for (int i = 0; i < nodeNum; i++) {
            // ノードiまでのコスト = ノードsrcからノードiまでのコスト + スタートノードからノードsrcまでのコスト
            int costToNode_i = costTable[src][i] + resultTable[src].cost;

            // コストテーブルのコストが０より大きく（経路がある）、現在のリザルトコストより小さい場合はコストを更新
            if (costTable[src][i] > 0 && costToNode_i < resultTable[i].cost) {
                resultTable[i].cost = costToNode_i;
                resultTable[i].parent = src;
            }
        }

        int minimumCostNode = -1;   // 確定していない中で最小のコストのノード
        int minimumCost = INT_MAX;  // 最小のコスト

        for (int i = 0; i < nodeNum; i++) {
            if (!resultTable[i].isFixed && resultTable[i].cost < minimumCost) {
                minimumCost = resultTable[i].cost;
                minimumCostNode = i;
            }
        }

        resultTable[minimumCostNode].isFixed = 1;
        src = minimumCostNode;
    } while (!resultTable[endNode].isFixed);  // ゴールノードが確定すると終了
}


// ダイクストラ法を実行した後のリザルトテーブルを表示する関数
void printResultTable(int nodeNum, NODEINFO resultTable[nodeNum]) {
    // リザルトテーブルを表示
    printf("ノード | コスト | 親ノード\n");
    for (int i = 0; i < nodeNum; i++) {
        printf("   %c   |   %2d   |    %c    \n", 'A' + resultTable[i].node,
               resultTable[i].cost, 'A' + resultTable[i].parent);
    }
}