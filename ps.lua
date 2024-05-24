-- inspect glyph geometry using FreeType and output PostScript

local outline=require"outline"

local function header()
print[[
%!PS-Adobe-2.0 EPSF-2.0
%%BoundingBox: (atend)
/V { 2 0 360 arc fill } bind def
/W { 3 0 360 arc fill } bind def
1 setlinecap
1 setlinejoin

%	axes
0 setlinewidth 0 0 1 setrgbcolor
-1000 0 moveto 1000 0 lineto 0 -1000 moveto 0 1000 lineto stroke
0 0 V
]]
end

local function footer()
print[[

showpage
%%EOF
]]
end

local function outliner(name)
	local nop=function () end
	return {name=name, B=nop, E=nop, L=nop, M=nop, Q=nop, C=nop}
end

---------------------------------------------------------------------------
local C=outliner("curves")
local x0,y0

function C.M(x1,y1)
	print(x1,y1,"moveto")
	x0,y0=x1,y1
end

function C.L(x1,y1)
	print(x1,y1,"lineto")
	x0,y0=x1,y1
end

function C.C(x1,y1,x2,y2,x3,y3)
	print(x1,y1,x2,y2,x3,y3,"curveto")
	x0,y0=x3,y3
end

function C.Q(x1,y1,x2,y2)
	C.C((x0+2*x1)/3,(y0+2*y1)/3,(x2+2*x1)/3,(y2+2*y1)/3,x2,y2)
	x0,y0=x2,y2
end

---------------------------------------------------------------------------
local P=outliner("polygons")

function P.M(x1,y1)
	print(x1,y1,"moveto")
end

function P.L(x1,y1)
	print(x1,y1,"lineto")
end

function P.C(x1,y1,x2,y2,x3,y3)
	print(x1,y1,"lineto",x2,y2,"lineto",x3,y3,"lineto")
end

function P.Q(x1,y1,x2,y2)
	print(x1,y1,"lineto",x2,y2,"lineto")
end

---------------------------------------------------------------------------
local V=outliner("vertices")

function V.M(x1,y1)
	print(x1,y1,"V")
end

function V.L(x1,y1)
	V.M(x1,y1)
end

function V.C(x1,y1,x2,y2,x3,y3)
	V.M(x1,y1)
	V.M(x2,y2)
	V.M(x3,y3)
end

function V.Q(x1,y1,x2,y2)
	V.M(x1,y1)
	V.M(x2,y2)
end

---------------------------------------------------------------------------
local W=outliner("vertices on curve")

function W.M(x1,y1)
	print(x1,y1,"W")
end

function W.L(x1,y1)
	W.M(x1,y1)
end

function W.C(x1,y1,x2,y2,x3,y3)
	W.M(x3,y3)
end

function W.Q(x1,y1,x2,y2)
	W.M(x2,y2)
end

---------------------------------------------------------------------------
local B=outliner("bbox")
local huge=math.huge
local xmin= huge
local xmax=-huge
local ymin= huge
local ymax=-huge

local function update(x,y)
	if (x<xmin) then xmin=x elseif (x>xmax) then xmax=x end
	if (y<ymin) then ymin=y elseif (y>ymax) then ymax=y end
end

function B.M(x1,y1)
	update(x1,y1)
end

function B.L(x1,y1)
	update(x1,y1)
end

function B.C(x1,y1,x2,y2,x3,y3)
	update(x1,y1)
	update(x2,y2)
	update(x3,y3)
end

function B.Q(x1,y1,x2,y2)
	update(x1,y1)
	update(x2,y2)
end

function B.E()
	if xmin==huge then return end
	local d=10
	print("%%BoundingBox:",xmin-d,ymin-d,xmax+d,ymax+d)
	print()
	print("%","bbox",xmin,xmax,ymin,ymax,d)
	print("0 setlinewidth 0.5 setgray")
	print(xmin,ymin,xmax-xmin,ymax-ymin,"rectstroke")
end

---------------------------------------------------------------------------
local D=outliner("data")

---------------------------------------------------------------------------
local function draw(g,f,t,b,e)
	local cb=function(w,...) t[w](...) end
	if t==D then
		cb=function(...) print("%",...) end
	end
	print()
	print("%",t.name)
	if b~="" then print(b) end
	outline(g,f,cb)
	if e~="" then print(e) end
end

local function inspect(g,f)
	print("%","glyph",g,f)
	draw(g,f,B,"","")
	draw(g,f,D,"","")
	draw(g,f,C,"0 setlinewidth 1 0.8 0.8 setrgbcolor","fill")
	draw(g,f,C,"3 setlinewidth 1 0.0 0.0 setrgbcolor","stroke")
	draw(g,f,P,"1 setlinewidth 0.8 setgray","stroke")
	draw(g,f,V,"0 setlinewidth 0.5 setgray","")
	draw(g,f,W,"0 setlinewidth 0.0 setgray","")
end

-- main
g = arg[1] or "&"
f = arg[2] or "/System/Library/Fonts/Times.ttc"
g = tonumber(g:match("^#(%-?%d+)$")) or g
header()
inspect(g,f)
footer()
