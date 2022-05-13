#pragma once

namespace Terra {
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			:m_Time(time)
		{
		}
		// allows a Timestep object to be used as a float implicitly
		operator float() const { return m_Time; }	

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};

}
