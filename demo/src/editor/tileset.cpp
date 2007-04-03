///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2004-2007 by The Allacrost Project
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 2. It is free software 
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

/*!****************************************************************************
 * \file    tileset.cpp
 * \author  Philip Vorsilak, gorzuate@allacrost.org
 * \brief   Source file for editor's tileset, used for maintaining a visible
 *          "list" of tiles to select from for painting on the map.
 *****************************************************************************/

#include "tileset.h"

using namespace std;
using namespace hoa_video;
using namespace hoa_script;
using namespace hoa_editor;

TilesetTable::TilesetTable(QWidget* parent, const QString& name)
	: Q3Table(parent, (const char*) name)
{	
	// Set some table properties.
	setReadOnly(true);
	setShowGrid(false);
	setSelectionMode(Q3Table::Single);
	setTopMargin(0);
	setLeftMargin(0);
	
	// Create filename from name.
	tileset_name = name;
	QString filename;
	filename = name;
	filename.append(".png").prepend("img/tilesets/");
	tiles.resize(256);
	for (int i = 0; i < 256; i++)
		tiles[i].SetDimensions(1.0f, 1.0f);
	
	if(!VideoManager->LoadMultiImage(tiles, filename.toStdString(), 16, 16))
		qDebug("LoadMultiImage failed to load tileset " + filename);

	// Set up the table.
	//int num_columns = visibleWidth() / TILE_WIDTH - 1;
	//setNumCols(num_columns);
	setNumCols(16);
	setNumRows(16);
	//setNumRows(static_cast<int> (ceil(
	//		static_cast<double> (tiles.size()) / static_cast<double> (num_columns))));
	for (int i = 0; i < numRows(); i++)
		setRowHeight(i, TILE_HEIGHT);
	for (int i = 0; i < numCols(); i++)
		setColumnWidth(i, TILE_WIDTH);

	// Read in tiles and create table items.
	QRect rectangle;
	QDir dir = QDir::current();
	for (int row = 0; row < 16; row++)
	{
		for (int col = 0; col < 16; col++)
		{
			QImageReader reader(filename, "png");
			rectangle.setRect(col * TILE_WIDTH, row * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
			reader.setClipRect(rectangle);
			QImage tile_img = reader.read();
			if (!tile_img.isNull())
			{
				// FIXME: conversion from image to pixmap isn't working, so save the tile and then read it for now
				//tile_pixmap.fromImage(tile_img);
				if (!tile_img.save(QString("tile%1.png").arg(col + row * 16)))
					qDebug("Saving error during tileset table creation");
				QPixmap tile_pixmap(QString("tile%1.png").arg(col + row * 16));
				setPixmap(row, col, tile_pixmap);
				dir.remove(QString("tile%1.png").arg(col + row * 16));
			} // image of the tile must not be null
			else
				qDebug(QString("%1").arg(reader.error()));
		} // iterate through the columns of the tileset
	} // iterate through the rows of the tileset
} // TilesetTable constructor

TilesetTable::~TilesetTable()
{
} // TilesetTable destructor



//*******************************************************
// DbTile class public functions follow
//*******************************************************
DbTile::DbTile()
{
} // DbTile default constructor

DbTile::DbTile(const QString& filename, int walkable)
{
	file_name = filename;
	walkability = walkable;
} // DbTile constructor

DbTile::~DbTile()
{
} // DbTile destructor



//*******************************************************
// TileDatabase class public functions follow
//*******************************************************
TileDatabase::TileDatabase() : _global_set(this)
{
} // TileDatabase default constructor

TileDatabase::TileDatabase(const QString& db_file_name) : _global_set(this)
{
	ScriptDescriptor read_data;
	read_data.OpenFile(db_file_name.toStdString(), SCRIPT_READ);

	// Read filenames and add tiles
	read_data.ReadOpenTable("tile_filenames");
	for (uint32 i = 1; i <= read_data.ReadGetTableSize(); i++)
	{
		read_data.ReadOpenTable(i);
		// read properties and insert tile
		QString file_name = QString::fromStdString(read_data.ReadString(0));
		int32 walkability = read_data.ReadInt(1);
		DbTile tile(file_name, walkability);
		_tiles.insert(std::pair<QString, DbTile> (file_name, tile));
		read_data.ReadCloseTable();

		// add tile to global set
		_global_set.AddTile(file_name);
	} // iterate through all the tile filenames
	read_data.ReadCloseTable();

	read_data.CloseFile();
} // TileDatabase constructor

TileDatabase::~TileDatabase()
{
	_tiles.clear();
} // TileDatabase destructor

void TileDatabase::Update(const QString& tile_dir_name)
{
	QDir tile_dir(tile_dir_name);	
	QStringList files = tile_dir.entryList("*.png");

	// Iterate through the tiles in the database and delete ones that do not exist
	for (std::map<QString, DbTile>::iterator it = _tiles.begin(); it != _tiles.end(); it++)
	{
		if (files.find((*it).first) == files.end())
		{
			// Remove from global set
			_global_set.RemoveTile((*it).first);
			_tiles.erase(it);
		} // tile not found
	} // go through all tiles in the database

	// Iterate through all files in the directory and add new ones
	for (QStringList::const_iterator it = files.begin(); it != files.end(); it++)
	{
		QString tile_file = *it;
		// if it does not exist in the database yet, add it
		if (_tiles.find(tile_file) == _tiles.end())
		{
			vector<StillImage> tile_vec;
//			if (!VideoManager->LoadMultiImage(tile_vec, tile_file.toStdString(), 16, 16))
//				qDebug("Failed to load multi-image.");
//			else
//				qDebug("Hooray! Seems to have worked...");
			DbTile new_tile(tile_file, 255);
			_tiles.insert(std::pair<QString, DbTile> (tile_file, new_tile));
			_global_set.AddTile(tile_file);
		} // tile not found
	} // go through all files in the directory
} // Update(...)

void TileDatabase::Save(const QString& file_name)
{
	ScriptDescriptor write_data;
	write_data.OpenFile(file_name.toStdString(), SCRIPT_WRITE);

	// Write tiles
	write_data.WriteComment("Stores names and properties of all tiles in the database");
	write_data.WriteBeginTable("tile_filenames");
	int index = 1;
	for(std::map<QString, DbTile>::iterator it = _tiles.begin(); it != _tiles.end(); it++)
	{
		write_data.WriteBeginTable(index);
		write_data.WriteString(0, (*it).second.file_name.ascii());
		write_data.WriteInt(1, (*it).second.walkability);
		write_data.WriteEndTable();
		index++;
	}
	write_data.WriteEndTable();

	write_data.CloseFile();
} // Save(...)

DbTile& TileDatabase::GetTile(const QString& tile_name)
{
	return _tiles[tile_name];
} // GetTile(...)

TileSet& TileDatabase::GetGlobalSet()
{
	return _global_set;
} // GetGlobalSet()

bool TileDatabase::TileExists(const QString& tile_name)
{
	return (_tiles.find(tile_name) != _tiles.end());
} // TileExists(...)



//*******************************************************
// TileSet class public functions follow
//*******************************************************
TileSet::TileSet(TileDatabase* db)
{
	_db = db;
} // TileSet constructor

TileSet::TileSet(TileDatabase* db, const QString& name)
{
	_db = db;
	_name = name;

	ScriptDescriptor read_data;
	read_data.OpenFile("dat/tilesets/tileset_" + name.toStdString() + ".lua", SCRIPT_READ);
	read_data.ReadOpenTable("tile_names");
	for(uint32 i = 1; i <= read_data.ReadGetTableSize(); i++)
	{
		QString tile_name = QString::fromStdString(read_data.ReadString(i));
		AddTile(tile_name);
	}
	read_data.ReadCloseTable();
	read_data.CloseFile();
} // TileSet constructor

TileSet::~TileSet()
{
	_db = NULL;
	_name = "";
	_tile_names.clear();
} // TileSet destructor

void TileSet::AddTile(const QString& tile_name)
{
	_tile_names.push_back(tile_name);
} // AddTile(...)

void TileSet::RemoveTile(const QString& tile_name)
{
	_tile_names.remove(tile_name);
} // RemoveTile(...)

DbTile& TileSet::GetTile(const QString& tile_name)
{
	return _db->GetTile(tile_name);
} // GetTile(...)

std::list<DbTile> TileSet::GetTiles() const
{
	std::list<DbTile> ret;
	
	for(std::list<QString>::const_iterator it = _tile_names.begin(); it != _tile_names.end(); it++)
	{
		ret.push_back(_db->GetTile(*it));
	} // iterate through all tiles in the database
	
	return ret;
} // GetTiles()

void TileSet::Save()
{
	if(_name == "") 
	{
		QMessageBox::warning(NULL,"Error","Tileset needs to have a name in order to save it!");
		return;
	} // filename must not be empty

	ScriptDescriptor write_data;
	write_data.OpenFile("dat/tilesets/tileset_" + _name.toStdString() +".lua", SCRIPT_WRITE);

	write_data.WriteBeginTable("tile_names");
	int index = 1;
	for (std::list<QString>::const_iterator it = _tile_names.begin(); it != _tile_names.end(); it++)
	{
		write_data.WriteString(index, (*it).ascii());
		index++;
	} // write all the tile filenames
	write_data.WriteEndTable();
	write_data.CloseFile();
} // Save()

void TileSet::SetName(const QString& name)
{
	_name = name;
} // SetName(...)

const QString& TileSet::GetName() const
{
	return _name;
} // GetName()
