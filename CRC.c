#include <stdio.h>

/* プロトタイプ宣言 */
// CRCを実装する関数
unsigned short exeCRC(unsigned short data[], int data_length);

// データ系列を表示する関数
void printResult(unsigned short data[], int data_length);

// ----- プロトタイプ宣言ここまで



int main(void){
    // データ系列（オリジナルデータ系列）
    unsigned short data[] = {
        0x1111,
        0x2222,
        0x3333,
        0x4444,
    };
    int data_length = sizeof(data) / sizeof(data[0]);

    // CRCを付加する前のデータ系列を表示
    printf("元データ系列: ");
    printResult(data, data_length);


    // CRC計算する
    unsigned short crc = exeCRC(data, data_length);
    printf("CRC: 0x%04X\n", crc);   // CRCを表示


    // CRCを付加したデータ系列（送信データ系列）
    unsigned short data2[] = {
        0x1111,
        0x2222,
        0x3333,
        0x4444,
        crc,    // CRCを付加
    };
    int data2_length = sizeof(data2) / sizeof(data2[0]);


    // CRCで符号化されたデータ系列を受信したと仮定してデータ系列の誤りの有無を確認
    unsigned short remainder = exeCRC(data2, data2_length);


    // 実行結果を表示
    printf("CRCを付加したデータ系列: ");
    printResult(data2, data2_length);
    
    // (受信したデータ/生成多項式)の余りが0なら誤りなし、それ以外なら誤りあり
    if (remainder == 0) {
        printf("誤りなし\n");
    } else {
        printf("誤り検出\n");
    }

    return 0;
}


// CRCを実装する関数
unsigned short exeCRC(unsigned short data[], int data_length) {
    unsigned short crc = 0x0000;  // CRCの初期値
    //  𝑥^16 + 𝑥^11 + 𝑥^4 + 𝑥 + 1  ->  (0b 1 0000 1000 0001 0011)  ->  (ox 10813)
    unsigned short poly = 0x0813;  // CRC-16の生成多項式

    for (int i = 0; i < data_length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 16; j++) {
            // 先頭ビットが1の場合は残りの15ビットと生成多項式をXOR
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;    // 先頭ビット以外をXORするためにcrcを左シフト
            } 
            // 先頭ビットが0の場合はcrcを左シフトしてXORはしない
            else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}


// データ系列を表示する関数
void printResult(unsigned short data[], int data_length) {
    for (int i = 0; i < data_length; i++) {
        printf("0x%04X ", data[i]);
    }
    printf("\n");
}