/******************************************************************************
 *
 *	Hero of Allacrost MapEditor class implementation
 *	Copyright (c) 2004
 *	Licensed under the GPL
 *
 *	Created by: Philip Vorsilak
 *	Filename: map_editor.cpp
 *
 *	$Id$
 *
 *	Description: This class controls and manipulates a map editor needed for
 *               editing Hero of Allacrost maps.
 *
 *****************************************************************************/

#include "map_editor.h"

using namespace std;
using namespace hoa_mapEd;

const QString APP_KEY = "/map_editor/";

MapEditor::MapEditor() : QMainWindow(0, 0, WDestructiveClose)
{
	// create the main widget and tile icons
	QSplitter *split = new QSplitter(this);
	tiles = new Tileset(split);
	map = new MapGrid(split);

	setCentralWidget(split);
	split->show();
	resize(600, 400);

	// create the statusbar
	statBar = new QStatusBar(this);
	
	// file menu creation
	fileMenu = new QPopupMenu(this);
	connect(fileMenu, SIGNAL(aboutToShow()), this, SLOT(fileMenuSetup()));
	menuBar()->insertItem("&File", fileMenu);
	
	// help menu creation
	helpMenu = new QPopupMenu(this);
	menuBar()->insertItem("&Help", helpMenu);
	helpMenu->insertItem("&Help", this, SLOT(helpHelp()), Key_F1);
	helpMenu->insertItem("&About", this, SLOT(helpAbout()));
	helpMenu->insertItem("About &Qt", this, SLOT(helpAboutQt()));
	
	mapObject = new hoa_map::MapMode(1);
	
	// FIXME: these settings don't seem to be working...? hmm
/*	QSettings settings;		// contains saveable settings, like recent files
	QString filename;		// file to add to list of recently used files
	for (int i = 0; i < MAX_RECENTFILES; ++i)
	{
		filename = settings.readEntry(APP_KEY + "File" + 
			QString::number(i + 1));
		if (!filename.isEmpty())
			masterRecentFiles.push_back(filename);
	} // puts saved files into a list of recently used files

	// updates the recent files list in the File menu
	if (masterRecentFiles.count())
		updateRecentFilesMenu();
*/	
	// loads the tileset for drag 'n' drop operation
	tileInit();
	
	// opens the most recently used file
//	if (!filename.isEmpty())
//		load(filename);
} // MapEditor constructor

MapEditor::~MapEditor()
{
	if (map != NULL)
		delete map;
	if (tiles != NULL)
		delete tiles;
	if (mapObject != NULL)
		delete mapObject;
} // MapEditor destructor

void MapEditor::closeEvent(QCloseEvent *)
{
    fileQuit();
} // closeEvent(...)

void MapEditor::fileMenuSetup()
{
	fileMenu->clear();
	fileMenu->insertItem("&New...", this, SLOT(fileNew()), CTRL+Key_N);
	fileMenu->insertItem("&Open...", this, SLOT(fileOpen()), CTRL+Key_O);
	int saveID = fileMenu->insertItem("&Save", this, SLOT(fileSave()), CTRL+Key_S);
	int saveAsID = fileMenu->insertItem("Save &As...", this, SLOT(fileSaveAs()));
	fileMenu->insertSeparator();
	int resizeID = fileMenu->insertItem("&Resize Map...", this, SLOT(fileResize()));
	fileMenu->insertSeparator();
	fileMenu->insertItem("&Quit", this, SLOT(fileQuit()), CTRL+Key_Q);
//	fileMenu->insertSeparator();

	if (map->getChanged())
	{
		fileMenu->setItemEnabled(saveID, true);
		fileMenu->setItemEnabled(saveAsID, true);
		fileMenu->setItemEnabled(resizeID, true);
	} // map has been changed, so those changes can be saved
	else
	{
		fileMenu->setItemEnabled(saveID, false);
		fileMenu->setItemEnabled(saveAsID, false);
		fileMenu->setItemEnabled(resizeID, false);
	} // map hasn't changed, no changes to be made, gray them out
} // fileMenuSetup()

void MapEditor::fileNew()
{
	if (eraseOK())
	{
		QCanvas *canvas = new QCanvas(this);
		map->setCanvas(canvas);
		fileResize();
	} // make sure an unsaved map is not lost
} // fileNew()

void MapEditor::fileOpen()
{
	// file to open
	QString fileName = QFileDialog::getOpenFileName(
		"data/maps", "Maps (*.hoa)", this, "file open",
		"HoA Map Editor -- File Open");

	// file must exist in order to open it
	if (!fileName.isEmpty())
		load(fileName);
} // openMap()
/*
void MapEditor::fileOpenRecent(int index)
{
	load(masterRecentFiles[index]);
} // fileOpenRecent(...)
*/
void MapEditor::fileSaveAs()
{
	// get the file name from the user
	QString fileName = QFileDialog::getSaveFileName(
		"data/maps", "Maps (*.hoa)", this, "file save",
		"HoA Map Editor -- File Save");
		
	if (!fileName.isEmpty())
	{
		int answer = 0;		// button pressed by user
		
		// ask to overwrite existing file
		if (QFile::exists(fileName))
			answer = QMessageBox::warning(
				this, "Overwrite File",
				QString("Overwrite\n\'%1\'?" ).arg(fileName),
				"&Yes", "&No", QString::null, 1, 1);
				
		if (answer == 0)
		{
			map->setFileName(fileName);
//			updateRecentFiles(fileName);  <-- fix this ************************
			fileSave();
			return;
		} // save the file
    } // make sure the file name is not blank
	
    statBar->message("Save abandoned", 5000);
} // fileSaveAs()

void MapEditor::fileSave()
{
	if (map->getFileName().isEmpty())
	{
		fileSaveAs();
		return;
    } // gets a file name if it is blank

    QFile file(map->getFileName());		// file to write to
	
    if (!file.open(IO_WriteOnly))
	{
		statBar->message(QString("\'%1\' is not writable").
			arg(map->getFileName()), 5000);
		return;
    } // make sure file is openable
    
    map->saveMap(file);		// actually saves the map
    file.close();

    setCaption(QString("%1").arg(map->getFileName()));
    //statBar->message(QString("Saved \'%1\'").arg(map->getFileName()),5000);
	statBar->message(QString("Hold your horses!"
		" Saving will be implemented soon..."), 5000);
} // fileSave()

void MapEditor::fileResize()
{
	bool ok_wpressed, ok_hpressed;	// TRUE = user pressed OK, else FALSE
	
	// get map width from user
	int width = QInputDialog::getInteger("Map Size...",
		"Enter map width (in tiles):", 0, 0, 1000, 1, &ok_wpressed, this);

	// get map height from user
	int height = QInputDialog::getInteger("Map Size...",
		"Enter map height (in tiles):", 0, 0, 1000, 1, &ok_hpressed, this);

	if (ok_wpressed && ok_hpressed)
	{
		map->canvas()->resize(width * TILE_WIDTH, height * TILE_HEIGHT);
		map->createGrid();
		map->setWidth(width);
		map->setHeight(height);
	} // only if the user entered both the height and the width
	else
		statBar->message("Invalid dimensions, no map created",5000);
} // fileResize()

void MapEditor::fileQuit()
{
	if (eraseOK())
	{	
		//saveOptions();		// saves window settings
		qApp->exit(0);
	} // checks to see if the map is unsaved
} // fileQuit()

bool MapEditor::eraseOK()
{
    if (map->getChanged())
	{
		switch(QMessageBox::warning(this, "Unsaved File",
			"The document contains unsaved changes\n"
			"Do you want to save the changes before exiting?",
			"&Save", "&Discard", "Cancel",
			0,		// Enter == button 0
        	2))		// Escape == button 2
		{
    		case 0: // Save clicked or Alt+S pressed or Enter pressed.
        		// save and exit
				fileSave();
				break;
			case 1: // Discard clicked or Alt+D pressed
				// don't save but exit
				break;
			default: // Cancel clicked or Escape pressed
    	    	// don't exit
				statBar->message("Save abandoned", 5000);
        		return FALSE;
	    } // warn the user to save
    } // map has been modified

    return TRUE;
} // eraseOK()

/*void MapEditor::updateRecentFiles(const QString &fileName)
{
    if (masterRecentFiles.find(fileName) == masterRecentFiles.end())
	{
		masterRecentFiles.push_back(fileName);
		if (masterRecentFiles.count() > MAX_RECENTFILES)
			masterRecentFiles.pop_front();
		updateRecentFilesMenu();
	} // the file must not already be in the list of recently used files
} // updateRecentFiles(...)

void MapEditor::updateRecentFilesMenu()
{
    for (int i = 0; i < MAX_RECENTFILES; ++i)
	{
		if (fileMenu->findItem(i))
			fileMenu->removeItem(i);
		if (i < int(masterRecentFiles.count()))
			fileMenu->insertItem(QString("&%1 %2").
				arg(i + 1).arg(masterRecentFiles[i]),
				this, SLOT(fileOpenRecent(int)), 0, i );
    } // loops through the whole list, updating if necessary
} // updateRecentFilesMenu()

void MapEditor::saveOptions()
{
    QSettings settings;		// contains saveable settings, like recent files
	
	// saves the list of recently used files
    for (int i = 0; i < int(masterRecentFiles.count()); ++i)
		settings.writeEntry(APP_KEY + "File" + QString::number(i + 1),
			masterRecentFiles[i]);
} // saveOptions()

void MapEditor::viewToggleGrid()
{
	// toggles the map's grid on or off
	map->viewToggleGrid();
} // viewToggleGrid()
*/
void MapEditor::helpHelp()
{
	statBar->message(QString("Dunno what to put here..."), 5000);
} // helpHelp()

void MapEditor::helpAbout()
{
    QMessageBox::about(this, "HoA Map Editor -- About",
		"<center><h1><font color=blue>Hero of Allacrost Map Editor<font>"
		"</h1></center>"
		"<center><h2><font color=blue>Copyright (c) 2004<font></h2></center>"
		"<p>A map editor created for the Hero of Allacrost project."
		" See 'http://www.allacrost.org/' for more details</p>");
} // helpAbout()

void MapEditor::helpAboutQt()
{
    QMessageBox::aboutQt(this, "HoA Map Editor -- About Qt");
} // helpAboutQt()

void MapEditor::load(const QString &fileName)
{
	statBar->message(QString("Hold your horses!"
		" Loading will be implemented soon..."), 5000);

/*	if (!fileName.isNull())
	{
		updateRecentFiles(fileName);
		map = new MapGrid(0, fileName);
	} // only update if we have a name for the map
	else
		map = new MapGrid(0); // <-- zero will make a floating map */
} // load(...)

void MapEditor::tileInit()
{
	QDir tileDir("img/tile/", "*.png");				// tile set directory
	vector<hoa_video::ImageDescriptor> map_tiles;	// vector of tiles
	hoa_video::ImageDescriptor imd;
	
	imd.width = 1;
	imd.height = 1;
	
	// make sure directory exists
	if (!tileDir.exists())
        	qWarning("Cannot find the tile directory");
	
	for (uint i = 0; i < tileDir.count(); i++)
	{
		(void) new QIconViewItem(tiles, tileDir[i], QPixmap("img/tile/" + tileDir[i]));
		imd.filename = "img/tile/" + string(tileDir[i].ascii());
		map_tiles.push_back(imd);
	}

	mapObject->SetMapTiles(map_tiles);
} // tileInit()
