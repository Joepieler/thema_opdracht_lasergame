// ==========================================================================
//
// File      : ButtonListener.hpp
// Copyright : bartvannetburg@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef BUTTON_LISTENER_HPP
#define BUTTON_LISTENER_HPP

/// \brief
/// A class containing the virtual buttonPressed function
class ButtonListener {
public:
    /// \brief
    /// This virtual function gets called by a button and gets overwritten in another class.
    virtual void buttonPressed( unsigned int & button_number ) = 0;
    
};

#endif
