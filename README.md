# Cross-Platform GUI Programming with wxWidgets
This project is merely to make the CD-ROM contents in the Book by Julian Smart and Kevin Hock avaiable in a moderized environment. That is to update the code to comply to WxWidgets 3.2 frameworks as 2.6.0 is seemingly *(and even >2.2.0 releases when studying the artworks)* the version that the authors used 20 odd years ago when writing the book.

## Introduction:

Run the flowwing commands to download and setup the build environment:
```
git clone https://github.com/JarlPed/Cross-Platform-GUI-Programming-with-wxWidgets.git
cd Cross-Platform-GUI-Programming-with-wxWidgets
cmake .
```



## Key Principles:
I'll strive to follow KISS, DRY and YAGNI principles (More about that here: https://workat.tech/machine-coding/tutorial/software-design-principles-dry-yagni-eytrxfhz1fla). Thus no fancy addtions are added in the source code. Only the minimum additions to get the applications to build and run with the least amount of effort is provided. Simple testing applications are written into **Testing** for the sole purpose to run the library files from generated in Chapters 3 and 20.

The Book is included in this repo [here](BOOK%20Cross-Platform%20GUI%20Programming%20with%20wxWidgets.pdf). As this book is in a public domain for free, I don't see any issue to include it.   

## Take Away points:

### Buildsystem
The build system used in this project is CMake, no build system whatsoever were included in the source files from CD-ROM. This poses a few issues if one wishes to build the examples and debug them straight away. For instance, the example in Chapter 3 the wxApp class is not used at all and therefore it can only be build as a library and can't be debugged without a test executable. The CMakeLists.txt file does not nessessarly follow "best practice" as it uses `link_directories()` and file GLOB for file fetching but it is OK in this context. 

For the purpose of simplicity only a single CMakeLists.txt is provided. I'll reserve the learning of using best practises for CMake and test coverage to other resources:
* CMake: https://www.packtpub.com/en-us/product/cmake-best-practices-9781803239729 & https://github.com/PacktPublishing/CMake-Best-Practices.
* Testing coverage (catch2):
  * Writing tests: https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md.
  * CMake integrations: https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md

### Updated syntaxes (ver 2.6 => 3.2) "Errata" & Additions:

Fortunately, WxWidgets maintains a list of depreacted classes in https://docs.wxwidgets.org/latest/deprecated.html.

#### Chapter 3
`SetBestFittingSize(size)` is replaced by `SetInitialSize(const wxSize& size)` in **chap03/fontctrl.cpp** 

#### Chapter 10
***Please note that this example is able to build, however, no window will appear since there is no defined wxFrame instance; The example is solely to demonstrate the wxArtProvider class***
The code section for **artprov.cpp** reads: 
```
// Initialization
bool MyApp::OnInit()
{
    ...

    wxArtProvider::PushProvider(new MyArtProvider);

    ...
    return true;
}
```
This will naturally not compile as the class `MyApp` is not defined in the scope, but reusing the following from Chapter 2 remidiates this:

```
class MyApp : public wxApp
{
public:
    // Called on application startup
    virtual bool OnInit();
};

DECLARE_APP(MyApp)
IMPLEMENT_APP(MyApp)
```

addtionally, `wxArtProvider::PushProvider()` is corrected to `wxArtProvider::Push()`.

The bitmaps used in the example does not exist in 3.2 and recent releases of WxWidgets (with exception of **art/helpicon.xpm**); thus the bitmaps in  
https://github.com/wxWidgets/wxWidgets/tree/WX_2_4_BRANCH/utils/helpview/src/bitmaps were copied to the **chap10** path.




#### Chapter 11
**chap11/dnd.cpp** is updated by the follwoing changes:
* `wxTHICK_FRAME` is replaced by `wxRESIZE_BORDER`
* `wxFlexGridSizer(4, 2)` is replaced by `wxFlexGridSizer(4, 2, 1)` The newer WxWidgets version requires two integers for vertical and horizontal gap sizing.  

#### Chapter 12

##### Taskbar (Notification area) example
**taskbar.cpp** resembles cloosely the Taskbar example provided from wxWidgets repository (https://github.com/wxWidgets/wxWidgets/tree/master/samples/taskbar). It is best to look past this example from the book since **tbtest.h** is omitted and that **wxDialog::Init** method is deprecated, therefore all initalizations take place in the **wxDialog::wxDialog()** constructor. I will further suggest the following code section for the constructors is not very easy to follow as it straight forwards the initalization tasks to the **wxDialog::Init** method anyways.

```
MyDialog::MyDialog(wxWindow* parent, const wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, const long windowStyle):
  wxDialog(parent, id, title, pos, size, windowStyle)
{
    Init();
}
```
Therefore the example from the wxWidgets repo is copied and used instead. 

##### Wizard Example
Missing file **wiztest.xpm** is copied from https://github.com/wxWidgets/wxWidgets/tree/WX_2_2_BRANCH/samples/wizard. **wiztest2.xpm** were created by simply inverting the colors of **wiztest.xpm** in GIMP editor

#### Chapter 19
`wxDocument::SaveObject()` may use arguments `wxOutputStream& stream` or `std::ostream& stream` depending on configuration of the WxWidgets installation instance. **chap19/doodle/doc.cpp**  

#### Chapter 20


##### Piped Process
`#include "wx/filename.h"` is appended in **chap20/pipedprocess/debugger.cpp** 

Since the bitmaps were not included in the CD-ROM for the debugger application, those xpm-bitmap icons are replaced by icons found in https://github.com/x64dbg/x64dbg/tree/development/src/gui/icons ( even https://github.com/NationalSecurityAgency/ghidra/tree/master/Ghidra/Debug/Debugger/src/main/resources/images may be used or even the icons used in WinDbg ).
*These icon/bitmap files has to be converted to xpm files using GIMP, etc.*
One may ofcourse use GIMP to make what ever icons one wishes to, and create a custom theme.

#### Riffle sample App
* `wxRect::Inside(wxPoint pt)` is replaced by `Contains(wxPoint pt)`.
* `wxSplitPath(wxString filePath, wxString& fullPath,  wxString& fileName,  wxString& fileExtension)` is replaced by `wxFileName::GetExt()`, etc.

## Final Notes:
Most of the examples uses bitmaps imports directly into the source files, a more modern approach is to handle this into a reperate resource file; e.g.  **sample.rc**.

The Book along with this repository brings a lot of value to learn how to work directly with wxWidgets and understand how things work under the hood. As you probably already know there are several WYSIWYG editors that allows you to create the basic source code for the GUI. Manually coding as presented in the Book and in this repo is not intended to replace that.