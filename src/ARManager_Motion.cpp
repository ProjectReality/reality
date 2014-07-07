#include "ARManager.hpp"


void ARManager::motionLoop(ARManager *ar)
{
	while (1)
	{
		if (ar->motionFrameChange)
		{
			cv::Mat fr;
			if (true)
			{
				boost::mutex::scoped_lock lock(ar->m_marker);
				fr = ar->frame.clone();
				ar->motionFrameChange = false;
			}
			if (ar->prev_frame.empty())
			{
				cvtColor(fr, ar->prev_frame, CV_RGB2GRAY);
				createHanningWindow(ar->hann, ar->frame.size(), CV_64F);
			}
			if (!ar->prev_frame.empty() && !ar->frame.empty())
			{
				// Calcul the motion
				cv::Mat curr, curr64f, prev64f;
				cvtColor(fr, curr, CV_RGB2GRAY);
				ar->prev_frame.convertTo(prev64f, CV_64F);
				curr.convertTo(curr64f, CV_64F);
				ar->motion = phaseCorrelate(prev64f, curr64f, ar->hann);
				ar->prev_frame = curr.clone();
				// End motion set
			}
		}
	}
}