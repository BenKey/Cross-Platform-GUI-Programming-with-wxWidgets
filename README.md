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
  * CMake integrations: https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md . 
  

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

This example basically attempts to use `gdb` (Gnu Debugger) executable located from from the **path** environmental variable. For windows, this is typically provided in a MSYS/CYGWIN environment. One may use the invoke the Visual C++ debugger (throu Visual Studio GUI) by: `devenv /debugexe <some-exe-file> /log logfile.txt`; this is . Alternatively, one may use `lldb` that is shipped with the LLVM compiler collection *(You may even install LLVM with the Visual Studio installer wizard!)*

For the CMakeLists.txt, you may choose to opt in for `gdb`, `lldb` or `devenv` with a log file watcher if MSVC is the target generator. 

#### Riffle sample App
* `wxRect::Inside(wxPoint pt)` is replaced by `Contains(wxPoint pt)`.
* `wxSplitPath(wxString filePath, wxString& fullPath,  wxString& fileName,  wxString& fileExtension)` is replaced by `wxFileName::GetExt()`, etc.

##### HTML Help files

The HTML Help files contains a **HelpBlocks** project file **riffle.wxh** that can be opened and edited. This is now a retired application last updated in 2019 and avaiable in full feature for free. For the convinence, the installation files are added to the **HelpBlockSetup** path of this repo along with registration name holder and key. This software is also available from http://anthemion.co.uk/helpblocks/index.htm. The successor to **HelpBlocks** is [**Jutoh**](https://www.jutoh.com/) and charges a one-time fee of 45 USD for the basic edition.




## Final Notes:
Most of the examples uses bitmaps imports directly into the source files, a more modern approach is to handle this into a reperate resource file; e.g.  **sample.rc**.

### HTML Help Content
For my personal taste, to organize and author HTML help content, I prefer to use **HelpBlocks** to organize the documents with indexes. However for creating the HTML files, I find it sufficient to a common word processor such as Microsoft Office Word, LibreOffice or OpenOffice. This is very neat for as nearly anybody can use these programs and create Help content very fast as they would make just about any other document. Once finished, one may just save the file as HTML document as a "printed" document. Most of HTML artifacts from these generated documents are supported see the [wxHTML documentation](https://docs.wxwidgets.org/stable/overview_html.html). Unsupported features are dropped in the viewer. Usually, the limitations are that the help files should be created very simple manner. Having text floating over images will not be displayed correctly, 

**Option**: One can create a post build command that activates the  *WinHelpCompiledFile.chm*, given that **HTML Help Workshop** is installed, the path to the compiler (really just a file zipper) is typically: `C:\Program Files (x86)\HTML Help Workshop\hhc.exe`. The syntax is `hhc.exe projFile.hhp` For convience, of the user selects the **USE_CHM** CMake flag 



### Mordernization

The Book along with this repository brings a lot of value to learn how to work directly with wxWidgets and understand how things work under the hood. As you probably already know there are several WYSIWYG editors that allows you to create the basic source code for the GUI. Manually coding as presented in the Book and in this repo is not intended to replace that.

*TODO*: I intent to use the Riffle App as a viehle to showcase the modernizations that can be made to an application. 

#### Multi-lingual applications

As demonstrated in a limited language selection in **chap16** example, multilingual applications can be transleted with `*.po` files. These files can be viewed and edited in e.g. [poedit](https://poedit.net/). When deciding on which languages to choose when making an application, you would likely consider the following:
* Supporting many languages introduces a lot of upkeep when updating the application with new features. This can however be automated away by a translation engine. In recent time, LLMs with billions of model parameters can be trained on millions of translation examples. One such open source model is [BigTranslate](https://github.com/ZNLP/BigTranslate). Using this however takes about 26 GB of disk space and scores worse than *Google Translate*. This is probably OK for meny item translations, but for HTML help files, this likely needs more careful though.
* What is your target audience or demography? - does it makes sence to support *Icelandic*?. For example, If your application is intendeded for very tech savvy users around the globe, chances are that *English* is more than sufficient.
* Profile your geographic marked - access [English proficeny score index](https://en.wikipedia.org/wiki/EF_English_Proficiency_Index) and evalute the need.
* What markets are actually available for you? There are several trade-embargoed countries (and even a list of prohibited persons), that is effected by a ban of distributiong software (dual-use) that might be used for military applications (such as ERP - belive is or not). If your software is applicable for this criteria, it may not be worthwile at the time of wrting to support *Russian* etc for this very reason.




#### Signing the application
Since the publication, several things has happened in the Cyber Sequirty space. It now seems that Microsoft and anti-virus vendors can't trust any installers on the web unless the developers provide a hash signature and pays them to whitelist their application. Creating the setup wizard program as is will make Microsoft belive this is a 0-day trojan and generally won't accept the user to launch it (or even keep the file on disk). Therefore, Microsoft Signtool whitin the Windows SDK comes handy to create a digital certificate of the app. 

An example to sign the application is provided here (and provided as an option for the Riffle app):
```
signtool sign
/f "path_to_your_certificate.pfx"
/p "your_certificate_password"
/t "http://time.windows.com/"
"file_to_sign.exe"
```
This has to be performed for *every* release that is created, and best suited to be automated by the  


creating the certificate *"path_to_your_certificate.pfx"* can be perfomred with powershell commands 
```
New-SelfSignedCertificate
  -Type Custom
  -Subject "CN=<Org Namespace>, O=<Org Name>, C=<Org-CountryCode>"
  -KeyUsage DigitalSignature
  -FriendlyName "My Friendly Cert Name"
  -CertStoreLocation "Cert:\CurrentUser\My"
  -TextExtension @("2.5.29.37={text}1.3.6.1.5.5.7.3.3", "2.5.29.19={text}")
Export-PfxCertificate
  -cert Cert:\CurrentUser\My\<Certificate Thumbprint>
  -FilePath <FilePath>.pfx
  -ProtectTo <Username or group name>
```
Please see [New-SelfSignedCertificate documentation](https://learn.microsoft.com/en-us/powershell/module/pki/new-selfsignedcertificate?view=windowsserver2022-ps) and 

#### Setup Wizards
The Book does cover Inno Setup scripts for creating distributable setup wizards for installing software on client machines. This is more or less in the same approach for NSIS Setup, but NSIS do have a more advanced script structure that avoids most of the workarounds that the `riffle\scripts\makesetup.sh` does to generate the Inno setup script file from `inno-top-file` and `inno-bottom-file` along with pesky UNIX to DOS lineendings due to using MSIS/CYGWIN. 

For this reason, I have provided an option in the CMakeLists.txt to create either a NSIS and/or Inno setup wizard.  

For the past two decades, Microsoft has released three diffent installer methods ()
* MSI -  Database structured approach to fetch install media remotely - full Win32 application.
  * WiX Toolset is commonly used for this, 
  * in CMakeLists.txt, using the CPack (included in CMake) module: 
  	  ```
    install(TARGETS coolstuff RUNTIME DESTINATION bin)
    
    set(CPACK_GENERATOR WIX)
    set(CPACK_PACKAGE_NAME "CoolStuff")
    set(CPACK_PACKAGE_VENDOR "Cool \"Company\"")
    set (CPACK_NSIS_MUI_ICON "@CMake_SOURCE_DIR@/Utilities/Release/CMakeLogo.ico")
    # set the add/remove programs icon using an installed executable
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\cmake-gui.exe")
    set (CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\CMakeInstall.bmp")
    include(CPack)
    ```
    Running the Cpack compiler after running CMake is fairly straight forward: `cpack -C Release --config ./<build directory>/CPackConfig.cmake`, or throu the IDE, CMake has a predefined target `PACKAGE`.
* ClickOnce - Only for .NET desktop applications. However Visual C++ applications are also covered if they are dependencies to the .NET project. ClickOnce automates self-updating applications with simplified security permissions. [read more here](https://learn.microsoft.com/en-us/visualstudio/deployment/clickonce-security-and-deployment?view=vs-2022)
  * - Needs App + Deployment Manifest files; using `Mage.exe`.
  Please see: https://learn.microsoft.com/en-us/visualstudio/deployment/walkthrough-manually-deploying-a-clickonce-application?view=vs-2022. 
* APPX - Virtual machine sandboxing for application. This reduces the external resources that the application can use from the Windows system. 
  * **MSIX Packaging Tool**
 
 `.msi` setup file generated from the [**Windows Installer**](https://learn.microsoft.com/en-us/windows/win32/msi/windows-installer-portal) also included with the Windows SDK. `.msi`. **MSIX Packaging Tool**

One should also consider if you intend that the application may not need Administrator premissions to install for the end-user. If the software is placed in **%APPDATA%** instead of **C:\Program Files**, you only use local user rights. This is usually encapsulated in the wizard telling the user to install for all users (requires admin privledges) or "just for me". 

Finally, Microsoft has now also a modern distribution channel for distributing software as a service; namely Microsoft Store. Both Free and Paid applications can be published there, but a commisioning fee for paid ones are introduced.


#### CI/CD
* GitHub Actions & Badge: You may run catch2 with `.\compiledTestExe --reporter xml`, save it somewhere accessable, then use an url assembeled from https://shields.io/badges/dynamic-xml-badge in your markdown `README.md`.
* CyberSecurity: CodeQL may be used to assess unsafe code sections, [unreachable code](https://codeql.github.com/codeql-query-help/go/go-unreachable-statement/) and [unused functions](https://codeql.github.com/codeql-query-help/go/go-unreachable-statement/). Similarly, one can create a badge for the `README.md` by generating a SARIF json document and possibly tally the respective levels with a simple python script.
* [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) issues can be reported by [cpplint](https://github.com/cpplint/cpplint). This is just a python script that can be modified to whichever coding conventions you like, and outputs an xml report. 