#pragma once

#define DELETE_COPY(class) \
class(const class&) = delete;\
class& operator=(const class&) = delete;

#define MOVE_CA(class, paramName, memberInitializers, constructorBody, assignmentBody) \
class& class::operator=(const class&& paramName) assignmentBody \
class(const class&& paramName) : memberInitializers constructorBody;

#define MOVE_A(class, paramName, memberInitializers, assignmentBody) MOVE_CA(class, paramName, memberInitializers, {}, assignmentBody)

/*\
class& class::operator=(const class&& paramName) assignmentBody \
class(const class&& paramName) : memberInitializers {};*/