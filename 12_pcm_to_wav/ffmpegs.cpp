#include "ffmpegs.h"
#include <QFile>
#include <QDebug>

#define PCMFilENAME "/Users/davis/Desktop/test.pcm"
#define WAVFilENAME "/Users/davis/Desktop/test.wav"

FFmpegs::FFmpegs()
{

}

void FFmpegs::pcm2wav() {
    // pcm转wav文件
    WAVHeader header;
    header.sampleRate = 48000;
    header.bitsPerSample = 32;
    header.numChannels = 1;
    header.audioFormat = AUDIO_FORMAT_FLOAT;
    header.blockAlign = header.bitsPerSample * header.numChannels / 8;
    header.byteRate = header.sampleRate * header.blockAlign;

    // 打开pcm文件
    QFile pcmFile(PCMFilENAME);
    if (!pcmFile.open(QFile::ReadOnly)) {
        qDebug() << "文件打开失败" << PCMFilENAME;
        return;
    }
    header.dataChunkDataSize = pcmFile.size();
    header.riffChunkDataSize = header.dataChunkDataSize
                               + sizeof (WAVHeader)
                               - sizeof (header.riffChunkId)
                               - sizeof (header.riffChunkDataSize);

    // 打开wav文件
    QFile wavFile(WAVFilENAME);
    if (!wavFile.open(QFile::WriteOnly)) {
        qDebug() << "文件打开失败" << WAVFilENAME;

        pcmFile.close();
        return;
    }

    // 写入头部
    wavFile.write((const char *) &header, sizeof (WAVHeader));

    // 写入pcm数据
    char buf[1024];
    int size;
    while ((size = pcmFile.read(buf, sizeof (buf))) > 0) {
        wavFile.write(buf, size);
    }

    // 关闭文件
    pcmFile.close();
    wavFile.close();
}
