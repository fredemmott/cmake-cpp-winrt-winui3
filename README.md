This is a proof-of-concept of using WinUI 3 without MSBuild, and with minimal
XAML.

`app.xaml` is required; window xaml files are avoided as they are impractical
to use without the XAML compiler, which is not currently available outside of
MSBuild.

![A screenshot of the app, showing a left hand navigation view](screenshot.png)
