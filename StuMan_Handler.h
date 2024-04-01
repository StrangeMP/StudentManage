#pragma once
// Process reqs, return a formatted JSON string in newly allocated memory.
// The user should free the memory when no longer need the responses to ensure memory safety.
const char *Handler(const char *reqs);