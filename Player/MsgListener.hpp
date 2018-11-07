// ==========================================================================
//
// File      : MsgListener.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef MSG_LISTENER_HPP
#define MSG_LISTENER_HPP

/// \brief
/// This struct gets used to split the message in two parts.
/// \details
/// The message gets split into the player part and the data part.
struct ir_msg {
	uint8_t player;
	uint8_t data;
};

/// \brief
/// This class constains the virtual msgReceived function.
class MsgListener {
public:
	/// \brief
	/// This function gets called by the MsgDecoder and sends the message to the class that overwrites this function.
	virtual void msgReceived( const ir_msg & msg ) = 0;
};

#endif