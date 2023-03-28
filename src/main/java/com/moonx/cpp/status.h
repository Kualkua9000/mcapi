#pragma once
#ifndef _STATUS_H_
#define _STATUS_H_

enum class mcStatus
{
    NotImplemented = -1,

    Success = 0,

    Complete,

    InvalidObject,
    InvalidDate,
    LocationRequired,
    ArgumentRange,
};


#endif
