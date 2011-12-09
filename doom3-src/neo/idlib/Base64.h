/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).  

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

//NEC. Base64.h contains the class idBase64 and any inline functions it has
#ifndef __BASE64_H__
#define __BASE64_H__

//NEC. idBase64 = a data type that stores a byte array into a *standardized* readable string form of a number in (radix-64) base64, base64 strings are regularly used to represent unicode characters, as a very simple form of encryption (like basic authentication over the Internet), and email attachments, however in this case they may just be for debugging memory or networking. *fun fact* base64 is one of the few data representation schemes that doesn't compress or encrypt data -- it only represents it in a different form
class idBase64 {
public:
				//NEC run the idBase64 private function 'Init'
				idBase64( void );
				
				//NEC run the idBase64 private function 'Init' and then use the defined operator '=' to assign the current idBase64 object the the passed string
				idBase64( const idStr &s );
				~idBase64( void );

	void		Encode( const byte *from, int size );
	void		Encode( const idStr &src );
	int			DecodeLength( void ) const; // minimum size in bytes of destination buffer for decoding
	int			Decode( byte *to ) const; // does not append a \0 - needs a DecodeLength() bytes buffer
	void		Decode( idStr &dest ) const; // decodes the binary content to an idStr (a bit dodgy, \0 and other non-ascii are possible in the decoded content)
	void		Decode( idFile *dest ) const;

	const char	*c_str() const;

	//NEC operator= = blindly copies any idStr to a idBase64 object (actually the private variable 'data'). the contents of the passed string 's' are not checked to see if they are base64
	void 		operator=( const idStr &s );

private:
	byte *		data;
	int			len;
	int			alloced;

	void		Init( void );
	void		Release( void );
	void		EnsureAlloced( int size );
};

//NEC run the idBase64 private function 'Init'
ID_INLINE idBase64::idBase64( void ) {
	
	//NEC run 'Init'
	Init();
}

//NEC run the idBase64 private function 'Init' and then use the defined operator '=' to assign the current idBase64 object the the passed string
ID_INLINE idBase64::idBase64( const idStr &s ) {
	
	//NEC run 'Init'
	Init();
	
	//NEC use the defined operator '=' to assign the current idBase64 object the the passed string
	*this = s;
}

ID_INLINE idBase64::~idBase64( void ) {
	Release();
}

ID_INLINE const char *idBase64::c_str( void ) const {
	return (const char *)data;
}

ID_INLINE void idBase64::Init( void ) {
	len = 0;
	alloced = 0;
	data = NULL;
}

ID_INLINE void idBase64::Release( void ) {
	if ( data ) {
		delete[] data;
	}
	Init();
}

//NEC idBase64::EnsureAlloced = used frequently to test the private idBase64 variable 'data' to see if it is a sufficent size. if it is not, the current data in 'data' is freed and new data exactly the size of 'size' is allocated and assigned to 'data'
ID_INLINE void idBase64::EnsureAlloced( int size ) {
	
	//NEC if the passed-by-value variable 'size' is larger than the amount allocated for private idBase64 variable 'data' (represented by the idBase64 private variable 'allocated') to use then it is freed
	if ( size > alloced ) {
		Release();
	}
	
	//NEC assign new data to idBase64 private variable 'data' exectly the size of the passed-by-value variable 'size'
	data = new byte[size];
	
	//NEC set the private idBase64 variable 'allocated' to reflect the new size of private idBase64 variable 'data'
	alloced = size;
}

//NEC idBase64::operator= = blindly copies any idStr to a idBase64 object (actually the private variable 'data'). the contents of the passed string 's' are not checked to see if they are base64
ID_INLINE void idBase64::operator=( const idStr &s ) {
	
	//NEC check if the private idBase64 variable 'data' has enough space to store the base64 string passed-by-reference variable '&s' with the trailing null ternimator. If not enough space exists the data pointed by 'data' gets freed and reallocated so as to perfectly fit 's'
	EnsureAlloced( s.Length()+1 ); // trailing \0 - beware, this does a Release
	
	//NEC copies the passed-by-reference variable '&s' to the idBase64 private variable 'data', including the trailing '\0'
	strcpy( (char *)data, s.c_str() );
	
	//NEC sets the idBase64 private variable 'len'
	len = s.Length();
}

#endif /* !__BASE64_H__ */
