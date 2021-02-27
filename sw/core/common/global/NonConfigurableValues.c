#include "NonConfigurableValues.h"

bool hostCompileFailed() { return hostCompileFailed_; }
void setHostCompileFailed() { hostCompileFailed_ = true; }

bool testsWereRun() { return testsWereRun_; }
void setTestsWereRun() { testsWereRun_ = true; }