function xmr { param([string]$target) xmake build $target && xmake run $target}

function mt { param([string]$target) make manual_test T=$target }
