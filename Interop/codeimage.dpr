library codecore;

{ This library exports the IImage interface defined in ImageTools
  Interally it relies on Vampyre Imaging Library loacated here:
  http://imaginglib.sourceforge.net/

	To compile:
  Place Vampyre Imaging sources in a subfolder named Imaging
  Delete or rename ImagingOptions.inc in the Imaging folder
	Compile on Linux using:
	
	fpc -FuImaging -FuImaging/JpegLib -FuImaging/ZLib codeimage.dpr
	
	Notes:
	IImage provides and abstract interface to working with images 
	IImage can be created using the ImageCreate function
	IImage loads and saves jpg, png, bmp, tga, and gif 
	IImage automatically converts loaded bits to A8R8G8B8 pixel format }

{$ifdef fpc}
{$mode delphi}
{$endif}

uses
	Math,
	ShareTools in 'sharetools.pas',
	StreamTools in 'streamtools.pas',
	ImageTools in 'imagetools.pas';
		
exports
  ImageCreate;	

begin
	SetExceptionMask([exInvalidOp, exDenormalized, exZeroDivide, 
		exOverflow, exUnderflow, exPrecision]); 
end.
