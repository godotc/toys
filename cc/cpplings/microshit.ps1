
function xmr {xmake build && xmake run}

function mt { param([string]$target) make manual_test T=$target }