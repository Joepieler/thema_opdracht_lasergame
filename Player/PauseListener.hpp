// ==========================================================================
//
// File      : PauseListener.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef PAUSE_LISTENER_HPP
#define PAUSE_LISTENER_HPP

/// \brief
/// This class contains the virtual function pauseDetected.
class PauseListener {
public:
	/// \brief
	/// The function gets called by the PauseDetector and the pause gets sent to the class that overwrites the function.
	virtual void pauseDetected( int pause_length ) = 0;
};

#endif // PAUSE_LISTENER_HPP