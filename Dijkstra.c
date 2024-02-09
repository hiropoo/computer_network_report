#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void makeCostTable(int nodeNum, int costTable[nodeNum][nodeNum], int pathNum,
                   FILE *fp);

int main(void) {
    int pathNum = -1;                          // パスの数
    int nodeNum = -1;                          // ノードの数
    int startNode = -1;                        // スタートノード
    int endNode = -1;                          // ゴールノード
    char fileName[] = "example_NodePath.txt";  // ファイル名
    FILE *fp = NULL;                           // ファイルポインタ

    if ((fp = fopen(fileName, "r")) == NULL) {
        printf("%s : ファイルを開けませんでした\n", fileName);
        return -1;
    }

    // パスの数とノードの数を入力
    fscanf(fp, "%d %d", &pathNum, &nodeNum);
    printf("pathNum = %d, nodeNum = %d\n", pathNum, nodeNum);

    // コストテーブル（経路表）を作成
    int costTable[nodeNum][nodeNum];  
    makeCostTable(nodeNum, costTable, pathNum, fp);

    // スタートノードとゴールノードを入力
    do {
        printf("スタートノードとゴールノードを入力してください\n");
        printf("スタート: ");
        scanf("%d", &startNode);
        printf("ゴール: ");
        scanf("%d", &endNode);
    } while (startNode < 0 || startNode >= nodeNum || endNode < 0 || endNode >= nodeNum);


    return 0;
}


// コストテーブル（経路表）を作成する関数
void makeCostTable(int nodeNum, int costTable[nodeNum][nodeNum], int pathNum, FILE *fp) {
    // コストテーブルの初期化
    for (int i = 0; i < nodeNum; i++)
        for (int j = 0; j < nodeNum; j++) costTable[i][j] = -1;

    // コストテーブル（経路表）の作成
    for (int i = 0; i < pathNum; i++) {
        // 1行（出発点 到着点 かかるコスト）ごと読み取り
        int src = -1, dst = -1, cost = -1;
        fscanf(fp, "%d %d %d", &src, &dst, &cost);
        // printf("src = %d, dst = %d, cost = %d\n", src, dst, cost);

        // 表にコストを記入。出発点と到着点が逆の場合も記入する
        costTable[src][dst] = cost;
        costTable[dst][src] = cost;
    }
}