/*
 * InstrumentPlayHandle.h - play-handle for driving an instrument
 *
 * Copyright (c) 2005-2014 Tobias Doerffel <tobydox/at/users.sourceforge.net>
 *
 * This file is part of LMMS - https://lmms.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */

#ifndef INSTRUMENT_PLAY_HANDLE_H
#define INSTRUMENT_PLAY_HANDLE_H

#include "PlayHandle.h"
#include "Instrument.h"
#include "NotePlayHandle.h"
#include "export.h"

class EXPORT InstrumentPlayHandle : public PlayHandle
{
public:
	InstrumentPlayHandle( Instrument * instrument, InstrumentTrack* instrumentTrack );

	virtual ~InstrumentPlayHandle()
	{
	}


	virtual void play( sampleFrame * _working_buffer )
	{
		// ensure that all our nph's have been processed first
		ConstNotePlayHandleList nphv = NotePlayHandle::nphsOfInstrumentTrack( m_instrument->instrumentTrack(), true );
		
		bool nphsLeft;
		do
		{
			nphsLeft = false;
			for( const NotePlayHandle * constNotePlayHandle : nphv )
			{
				NotePlayHandle * notePlayHandle = const_cast<NotePlayHandle *>( constNotePlayHandle );
				if( notePlayHandle->state() != ThreadableJob::Done && ! notePlayHandle->isFinished() )
				{
					nphsLeft = true;
					notePlayHandle->process();
				}
			}
		}
		while( nphsLeft );
		
		m_instrument->play( _working_buffer );
	}

	virtual bool isFinished() const
	{
		return false;
	}

	virtual bool isFromTrack( const Track* _track ) const
	{
		return m_instrument->isFromTrack( _track );
	}


private:
	Instrument* m_instrument;

} ;

#endif
