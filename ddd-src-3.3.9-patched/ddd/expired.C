// $Id$ -*- C++ -*-
// Check for expiration date

// Copyright (C) 1996 Technische Universitaet Braunschweig, Germany.
// Written by Andreas Zeller <zeller@gnu.org>.
// 
// This file is part of DDD.
// 
// DDD is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// DDD is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public
// License along with DDD -- see the file COPYING.
// If not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// 
// DDD is the data display debugger.
// For details, see the DDD World-Wide-Web page, 
// `http://www.gnu.org/software/ddd/',
// or send a mail to the DDD developers <ddd@gnu.org>.

char expired_rcsid[] = 
    "$Id$";

#include "expired.h"

#include "version.h"
#include <stdio.h>
#include <time.h>
#if !HAVE_DIFFTIME
inline double difftime(time_t time1, time_t time0) { return time1 - time0; }
#endif


//-----------------------------------------------------------------------------
// Check expiration date
//-----------------------------------------------------------------------------

// Return expiration time.  0: no expiration time, -1: invalid expiration time.
static time_t ddd_expiration_time()
{
    string expiration_date = DDD_EXPIRES;
    if (expiration_date.empty())
    {
	// No expiration date given
	return 0;
    }

    int year  = 0;
    int month = 0;
    int day   = 0;
    sscanf(expiration_date.chars(), "%d-%d-%d", &year, &month, &day);
    if (year == 0 || month == 0 || day == 0)
	return -1;

    struct tm expiration_tm;
    expiration_tm.tm_sec   = 0;
    expiration_tm.tm_min   = 0;
    expiration_tm.tm_hour  = 0;
    expiration_tm.tm_mday  = day;
    expiration_tm.tm_mon   = month - 1;
    expiration_tm.tm_year  = year - 1900; // No, this is no Y2K problem.  -AZ
    expiration_tm.tm_wday  = -1;
    expiration_tm.tm_yday  = -1;
    expiration_tm.tm_isdst = -1;
    
    time_t expiration_time = mktime(&expiration_tm);
    if (expiration_time < 0)
	return -1;

    return expiration_time;
}

string ddd_expiration_date()
{
    time_t expiration_time = ddd_expiration_time();
    if (expiration_time == 0)
	return "";		// No expiration
    else if (expiration_time < 0)
	return "the epoch";	// Invalid time

    string pretty_expiration_date;

#if HAVE_STRFTIME
    char buffer[1024];
    strftime(buffer, sizeof buffer, "%A, %Y-%m-%d, at %H:%M", 
	     localtime(&expiration_time));
    pretty_expiration_date = buffer;
#elif HAVE_ASCTIME
    pretty_expiration_date = asctime(localtime(&expiration_time));
    pretty_expiration_date.gsub("\n", "");
#else
    pretty_expiration_date = DDD_EXPIRES;
#endif

    return pretty_expiration_date;
}

bool ddd_expired()
{
    time_t expiration_time = ddd_expiration_time();
    if (expiration_time == 0)
    {
	// No expiration time -- this never ever expires.
	return false;
    }
    else if (expiration_time < 0)
    {
	// Invalid expiration time -- consider as expired.
	return true;
    }

    time_t current_time = time((time_t*)0);
    if (current_time < 0)
    {
	// No current time -- consider as expired.
	return true;
    }

    double seconds_since_expiration = difftime(current_time, expiration_time);
    if (seconds_since_expiration <= 0)
    {
	// Still within expiration date
	return false;
    }

    // Expired.
    return true;
}
