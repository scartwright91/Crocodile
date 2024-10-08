#pragma once

#include <iostream>
#include <chrono>
#include <thread>

namespace Crocodile
{
	class Clock {
		public:
			Clock() : m_lastFrameTime(std::chrono::high_resolution_clock::now()) {}

			void tick() {
				auto currentTime = std::chrono::high_resolution_clock::now();
				std::chrono::duration<float> delta = currentTime - m_lastFrameTime;
				m_lastFrameTime = currentTime;
				if (m_firstFrame)
				{
					m_deltaTime = 0.0f;
					m_firstFrame = false;
					return;
				}
				m_deltaTime = delta.count();
			}

			float inline getDeltaTime() { return (float)m_deltaTime; };
			float inline getFPS() { return 1.f / m_deltaTime; };

		private:
			bool m_firstFrame = true;
			float m_deltaTime;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_lastFrameTime;
	};
}
