/*
  Mouse.h

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SMH_h
#define SMH_h

#include <Arduino.h>
#include <String.h>

#include "Schedule.h"
#include "Serializer.h"
#include "Commands.h"
#include "TempSensors.h"

class SMHClass
{
private:
	String logfile_path;
	Print * printer;
	Commands * commands;
	Serializer * serializer;
	Schedulers * schedulers;
public:
	SMHClass();

 	TempSensors * tempsensors;

	void set_logfile_path(String);
	void set_printer(Print &);
	void loop(void);
};

extern SMHClass SMH;

#endif