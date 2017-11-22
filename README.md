# YUV处理

## 各目录代码说明
- yuv-444p-to-420p: yuv444p转换为yuv420p
- yuv-pure-color: 生成指定颜色的纯色的yuv444p图片
- yuv-file-merge: 将多个yuv文件合并为一个
- yuv-overlay/yuv420: yuv420p格式的2张图片叠加，透掉黑色
- yuv-overlay/yuv444: yuv444p格式的2张图片叠加，透掉黑色
- yuv-overlay/yuv444_420: 底图yuv420p，叠加yuv444p格式的图片。底图先转换为yuv444p再处理。
- yuv-overlay/yuv444_420_direct: 底图yuv420p，叠加yuv444p格式的图片。直接叠加。

## yuv-overlay
此目录下各项为以不同yuv格式将一张文字图片叠加到背景图片上。背景图片为一张纯色的bmp图片，文字bmp图片为firework制作（注意不要消除锯齿）。之后通过ffmpeg将bmp图片转换为需要的yuv格式图片。各种不同yuv格式的叠加效果如下：

1.文字图片及背景图片都是yuv420p格式，输出格式yuv420p。

![image](https://raw.githubusercontent.com/findAway/yuv-processing/master/yuv-overlay/overlay_420.bmp)

2.文字图片及背景图片都是yuv444p格式，输出格式yuv444p。

![image](https://raw.githubusercontent.com/findAway/yuv-processing/master/yuv-overlay/overlay_444.bmp)

3.文字为yuv444p格式，背景为yuv420p格式，输出格式yuv420p。有2种叠加方式。一种是直接叠加，另一种是背景转换为yuv444p格式后再叠加，叠加后再转换为yuv420p输出。

![image](https://raw.githubusercontent.com/findAway/yuv-processing/master/yuv-overlay/overlay_420_444.bmp)

从叠加后的输出图像效果看，上面第2种最好，第3种次之，第1种最差。

