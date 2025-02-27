#ifndef __MOVIE_H
#define __MOVIE_H

/*
 *
 *  Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers,
 *  and the Aleph Bet developers.
 *
 *  Aleph Bet is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Aleph Bet is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *  This license notice applies only to the Aleph Bet engine itself, and
 *  does not apply to Marathon, Marathon 2, or Marathon Infinity scenarios
 *  and assets, nor to elements of any third-party scenarios.
 *
 */

#include "cseries.hpp"
#include "OGL_FBO.hpp"
#include <memory>
#include <string.h>
#include <vector>
#include <SDL_thread.h>

class Movie
{
public:
	static Movie *instance() { 
		static Movie *m_instance = nullptr;
		if (!m_instance
				) m_instance = new Movie(); 
		return m_instance; 
	}
	
	void PromptForRecording();
	void StartRecording(std::string path);
	bool IsRecording();
	void StopRecording();
	long GetCurrentAudioTimeStamp();
	
	enum FrameType {
	  FRAME_NORMAL,
	  FRAME_FADE,
	  FRAME_CHAPTER
	};
	void AddFrame(FrameType ftype = FRAME_NORMAL);

private:
  
  std::string moviefile;
  SDL_Rect view_rect;
  SDL_Surface *temp_surface;
  
  std::vector<uint8> videobuf;
  std::vector<uint8> audiobuf;
  int in_bps;
  
  struct libav_vars *av;
  
  SDL_Thread *encodeThread;
  SDL_sem *encodeReady;
  SDL_sem *fillReady;
  bool stillEncoding;

#ifdef HAVE_OPENGL
  std::unique_ptr<FBO> frameBufferObject;
#endif
  
  Movie();  
  bool Setup();
  static int Movie_EncodeThread(void *arg);
  void EncodeThread();
  void EncodeVideo(bool last);
  void EncodeAudio(bool last);
  void ThrowUserError(std::string error_msg);
};
	
#endif
