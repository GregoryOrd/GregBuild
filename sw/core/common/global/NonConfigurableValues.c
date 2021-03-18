#include "NonConfigurableValues.h"

bool hostCompileFailed() { return hostCompileFailed_; }
void setHostCompileFailed() { hostCompileFailed_ = true; }

bool testBuild() { return testBuild_; }
void setTestBuild(bool run) { testBuild_ = run; }