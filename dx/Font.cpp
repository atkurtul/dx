#include "Font.h"
#include "Logger.h"
Font::Font()
{
}
Font::Font(const char* path)
{
	String data;
	std::ifstream s(path);
	data.getline(s);
	line = data.Substring(data.IndexOf("size") + 5, 3).tofloat();
	data.getline(s);
	data.getline(s);
	uint i = data.IndexOf('"') + 1;
	Atlas = Texture(data.Substring(i, data.IndexOf('"', i) - i).c_str());
	float scale = 256.f / Atlas.width;
	line *= scale;
	data.getline(s);
	while (data.getline(s))
	{
		Char c;
		int id = data.Substring(data.IndexOf("id") + 3, 3).toint();
		float x = data.Substring(data.IndexOf("x") + 2, 3).tofloat();
		float y = data.Substring(data.IndexOf("y") + 2, 3).tofloat();
		float width = data.Substring(data.IndexOf("width") + 6, 3).tofloat();
		float height = data.Substring(data.IndexOf("height") + 7, 3).tofloat();
		float xoff = data.Substring(data.IndexOf("xoffset") + 8, 3).tofloat();
		float yoff = data.Substring(data.IndexOf("yoffset") + 8, 3).tofloat();
		c.advance = data.Substring(data.IndexOf("xadvance") + 9, 3).tofloat();
		float x1 = (x + width) / Atlas.width;
		float y1 = (y + height) / Atlas.height;
		x /= Atlas.width;
		y /= Atlas.height;
		c.quad.Generate();
		width		*= scale;
		height		*= scale;
		xoff		*= scale;
		yoff		*= scale;
		c.advance	*= scale;
		float o = line - height - yoff;

		Array<vec4> buffer =
		{
			{xoff, o,					x, y1	},
			{xoff, height + o,			x, y	},
			{width + xoff, o,			x1, y1	},
			{width + xoff, height + o,	x1, y	},
			{width + xoff, o,			x1, y1	},
			{xoff, height + o,			x, y	}
		};
		c.quad.Generate();
		c.quad.Buffer(buffer, 1);
		charmap[id] = c;
	}
}
