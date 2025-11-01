# dgVoodoo2

*I don't have time for this project so I put it into archived state for a while. There is no point to have new issue entries waiting for answers in vain.*

Glide/DirectX implementation on D3D11/12

<ul>
<li>Play your old Glide and DirectX games even on the latest, modern version of Windows.</li>
<li>Glide, Glide2, Glide3 and Glide3 Napalm versions are available</li>
<li>DirectDraw, D3D3/5/6/7, D3D8 and D3D9 are all available too</li>
<li>Good compatiblity with the original libraries, I think.</li>
<li>You can force the resolution and even MSAA for both Glide and DirectX which can really improve the appearance of games with hardwired low-resolution support.</li>
<li>Support for hw/driver compatibility with some really old videocards like GF4 Ti4800 and ATI Radeon 8500 is also available for rare games needing that.</li>
<li>You can force windowed appearance and simple color-management and dithering (retro feeling) as postprocess effects.</li>
<li>For really old games running at their original low resolution you can upscale the image with various resampling filters like bicubic and lanczos. Or just apply simple pixel multiplication.</li>
<li>You get a lot of configurable elements, either through the GUI CPL app or manually with a texteditor, if you know what you are doing.</li>
</ul>

Thanks for the Vogons community for providing me useful feedbacks!

<hr>

You have an old 32bit game or application relying on an old graphics API and you might want to port/recompile it to a modern 64 bit platform but you don't want to
mess with porting the rendering code to a modern graphics API?  
<ul>
<li>You can easily do the graphics part of the porting with 64 bit dgVoodoo graphics API dlls that
you can freely ship with your application.</li>
<li>In the first step you can port it to plain x64 to see if it works and then compile it to ARM64 or ARM64EC if you use
components that have no native ARM64 versions.</li>
See the documentation in dgVoodoo API 2.86.3 (or newer) package.
</ul>
