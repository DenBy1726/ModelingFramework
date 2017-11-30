#pragma once
namespace Model
{

	struct Time
	{
	public:
		int time = 0;
		Time()
		{

		}
		Time(int val)
		{
			time = val;
		}
		operator int()
		{
			return time;
		}

	};
}