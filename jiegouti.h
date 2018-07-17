typedef struct _TWavHeader
{
        int rId;    //标志符（RIFF）
        int rLen;   //数据大小,包括数据头的大小和音频文件的大小
        int wId;    //格式类型（"WAVE"）
        int fId;    //"fmt"

        int fLen;   //Sizeof(WAVEFORMATEX)

        short wFormatTag;       //编码格式，包括WAVE_FORMAT_PCM，WAVEFORMAT_ADPCM等
        short nChannels;        //声道数，单声道为1，双声道为2
        int nSamplesPerSec;   //采样频率
        int nAvgBytesPerSec;  //每秒的数据量
        short nBlockAlign;      //块对齐
        short wBitsPerSample;   //WAVE文件的采样大小
        int dId;              //"data"
        int wSampleLength;    //音频数据的大小
}TWavHeader;