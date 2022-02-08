::mp3转pcm
::ffmpeg -y -f s16be -ac 1 -ar 16000 -acodec pcm_s16le  -i "%%a"  "output\%%~na.mp3"

@echo off

::在下方设置要处理的视频或音频格式，这里列出了一些主要的视频格式
set Ext=*.mp3

md output

echo 开始转换

::在下方设置输出格式，这里输出为mp4，可自行更改
for %%a in (%Ext%) do (
	echo 正在转换：%%a
	ffmpeg -y -i "%%a" -acodec pcm_s16le -f s16le -ac 1 -ar 16000 "output\%%~na.pcm"
)

echo 转换完成

pause
