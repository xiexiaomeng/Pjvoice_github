typedef struct _TWavHeader
{
        int rId;    //��־����RIFF��
        int rLen;   //���ݴ�С,��������ͷ�Ĵ�С����Ƶ�ļ��Ĵ�С
        int wId;    //��ʽ���ͣ�"WAVE"��
        int fId;    //"fmt"

        int fLen;   //Sizeof(WAVEFORMATEX)

        short wFormatTag;       //�����ʽ������WAVE_FORMAT_PCM��WAVEFORMAT_ADPCM��
        short nChannels;        //��������������Ϊ1��˫����Ϊ2
        int nSamplesPerSec;   //����Ƶ��
        int nAvgBytesPerSec;  //ÿ���������
        short nBlockAlign;      //�����
        short wBitsPerSample;   //WAVE�ļ��Ĳ�����С
        int dId;              //"data"
        int wSampleLength;    //��Ƶ���ݵĴ�С
}TWavHeader;