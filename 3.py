from pydub import AudioSegment
import numpy as np

def convert_mp3_to_header(mp3_file, header_file, array_name):
    # 加载 MP3 文件
    audio = AudioSegment.from_mp3(mp3_file)
    
    # 将音频转换为单声道
    audio = audio.set_channels(1)
    
    # 将音频转换为 8 位 PCM 格式
    audio = audio.set_sample_width(1)
    
    # 获取音频数据
    raw_data = np.array(audio.get_array_of_samples(), dtype=np.uint8)
    
    # 生成 C++ 头文件
    with open(header_file, 'w') as f:
        f.write(f'#ifndef {array_name.upper()}_H\n')
        f.write(f'#define {array_name.upper()}_H\n\n')
        f.write(f'const uint8_t {array_name}[] = {{\n')
        for i, sample in enumerate(raw_data):
            if i % 12 == 0:
                f.write('\n    ')
            f.write(f'0x{sample:02X}, ')
        f.write('\n};\n\n')
        f.write(f'const size_t {array_name}_len = {len(raw_data)};\n\n')
        f.write(f'#endif // {array_name.upper()}_H\n')

# 使用示例
convert_mp3_to_header('input.mp3', 'music_data.h', 'musicData')