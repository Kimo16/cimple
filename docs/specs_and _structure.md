

## Basic Version (v1.0)

The first release will include all the basic functionalities (i.e. the minimal part of our project).
It was decided to separate the model from the view and to make them communicate through the controller only.

After launching Cimple, the user can open one or several images. He will be able to edit them through the console commands.

**Different formats.** The user can open images of different formats as .jpg, .png and bitmap. He can also choose the format while saving the image.

**Selecting zone.** The user is able to pass a command *select* and then select a zone by clicking on the image and dragging it to modify the selection. Upon releasing the mouse the chosen zone will be selected and the user will be able to apply changes to it.

**Cut, copy and paste.** The user may also cut or copy the selected region and then paste wherever he wishes, even on another image. If no region is selected, Cimple pass in select mode.

**Image modifications.**

  * Transformations
    * Vertical and horizontal symetries
    * Image rotations mod90
  * Frame modifications
    * Resize working area (extending or reducing  the number of pixels the user is working on)
    * Modify the image size
  * Color modifications
    * Fill a selected region with a color
    * Replacing a color by another color in a selected region
    * Apply the negative effect on a selected region
    * Apply a Grey & White effect on a selected region
    * Apply a Black & White effect on a selected region


## Extended Version (v2.0)

For now, the extensions that we consider adding are :

**Scripts.** The user will be able to write commands in a separate file and then apply all of them to a chosen image.

**Scripts editor.** In order to improve the working flow and give the user a possibility to edit his scripts without closing the program, we will add a *edit_script* command. Once this command called, the main program will launch a process with a text editor ($EDITOR and nano if not defined). The user will be able to modify his script and on closing his editor, the main Cimple process will continue.

**Undo/redo.** The program will allow to revert the last applied change, and recursively the one before (etc...) or redo changes that were done (undo the undos).

## Project structure

The project main part consists of two separate parts, the model and the view, linked by a controller.
Here is what the directory tree would look like :

```sh
cimple
├── const.mk
├── docs
├── include
├── LICENSE
├── Makefile
├── README.md
├── scripts
│   ├── config_uncrustify.cfg
│   ├── uncrustify_setup.sh
│   └── uncrustify.sh
├── src
│   ├── controller
│   │   └── cmd_line.c
│   ├── main.c
│   ├── model
│   │   ├── in.c
│   │   ├── m_color.c
│   │   ├── m_frame.c
│   │   ├── m_transform.c
│   │   ├── out.c
│   │   └── parse.c
│   └── view
│       ├── cmd_view.c
│       ├── event.c
│       └── window.c
└── tests
```

Short description of these directories :
  * build :
  temporary files / files created by compiling the code (i.e. .o etc).
  * docs :
  documentation on how to use the program.
  * include :
  contains all the headers
  * lib :
  all library files (SDL etc)
  * tests :
  all the tests needed, including the ones for Travis CI.
  * src :
  main source files, see details below.
  * scripts :
  scripts needed for developping

### Source files

Here are some brief explanations on the source files and what they will do.

* view :
  * **cmd_view.c :**
     File responsible for the displays (such as coordinates while selecting region) and the input in the console.
  * **event.c :**
     Event handler for mouse clicks on displayed images.
  * **window.c :**
    Displaying images on the screen.
* model :
  * **in.c :**
    Opening/importing existing image.
  * **out.c :**
    Saving image on the machine.
  * **m_color.c :**
    Contains all the color modification functions.
  * **m_frame.c :**
    Contains all the window and frame changing functions.
  * **m_transform.c :**
    Contains symmetry and rotation functions.
  * **parse.c :**
    Gets an input line from the controller and parses it returning command structure.
* controller :
  * **cmd_line.c :**
    manage view and model calls.
* **main.c :**
    Main program.

### Crash resistance  :
  * On every launch the program searches for save.tmp and if it's found, the program opens it.
  * In case the program exits normally, save.tmp is deleted.
  * In case the program crashes, save.tmp will be opened without data loss of current work.

### Programs to use

* Flex
* Uncrustify
