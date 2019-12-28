#include "MAX30100_BeatDetector.h"
#include "timer3.h"
#include "usart.h "

BeatDetectorState Beatstate;//�������״̬

float threshold;	
float beatPeriod;
float lastMaxValue;
uint32_t tsLastBeat;

void BeatDetector()
{
    Beatstate=BEATDETECTOR_STATE_INIT;
    threshold=BEATDETECTOR_MIN_THRESHOLD;
    beatPeriod=0;
    lastMaxValue=0;
    tsLastBeat=0;

}

u8 addSample(float sample)
{
    return checkForBeat(sample);
}

float getRate()
{
    if (beatPeriod != 0) {
        return 1 / beatPeriod * 1000 * 60;
    } else {
        return 0;
    }
}

float getCurrentThreshold()//float BeatDetector::getCurrentThreshold()
{
    return threshold;
}

//�������״̬
// ״̬��
/********************************************************************************/
//
//									lastMaxValue=0
//			t>2000 ms							sample>threshold
// Init----------------->wait------------------>follow slope-----
//											 ��															 ��			|	
//												|																|			|	sample<threshold
//												|				    sample+30>threshold	|			|	
//												|																| 		|			 																									
//												|																|			V
//												-----masking	<---------------maybe detected
//						      t-tsLastBeat>200ms		sample+30<threshold
//																					beatDetected=1
//																				lastMaxValue = sample
/********************************************************************************/

u8 checkForBeat(float sample)
{
		
    u8 beatDetected = 0;
	  float delta;
		
    switch (Beatstate) 
			{
        case BEATDETECTOR_STATE_INIT:
            if (millis() > BEATDETECTOR_INIT_HOLDOFF) 
						{
							Beatstate = BEATDETECTOR_STATE_WAITING;
            }
            break;

        case BEATDETECTOR_STATE_WAITING:
            if (sample > threshold) 
						{
                threshold = min(sample, BEATDETECTOR_MAX_THRESHOLD);
								
									Beatstate = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
						}

            if (millis() - tsLastBeat > BEATDETECTOR_INVALID_READOUT_DELAY) 
						{
                beatPeriod = 0;
                lastMaxValue = 0;
            }

            decreaseThreshold();
            break;

        case BEATDETECTOR_STATE_FOLLOWING_SLOPE:
            if (sample < threshold) 
						{

									Beatstate = BEATDETECTOR_STATE_MAYBE_DETECTED;
						} 
						else 
						{
                threshold = min(sample, BEATDETECTOR_MAX_THRESHOLD);
            }
            break;

        case BEATDETECTOR_STATE_MAYBE_DETECTED:
            if (sample + BEATDETECTOR_STEP_RESILIENCY < threshold) 
						{
								beatDetected = 1;
                lastMaxValue = sample;
								Beatstate = BEATDETECTOR_STATE_MASKING;
							 delta = millis() - tsLastBeat;
                if (delta) 
								{
                    beatPeriod = BEATDETECTOR_BPFILTER_ALPHA * delta +
                            (1 - BEATDETECTOR_BPFILTER_ALPHA) * beatPeriod;
                }

                tsLastBeat = millis();
            } 
						else 
						{

									Beatstate = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
						}
            break;

        case BEATDETECTOR_STATE_MASKING:
            if (millis() - tsLastBeat > BEATDETECTOR_MASKING_HOLDOFF) 
						{
							   Beatstate = BEATDETECTOR_STATE_WAITING;
            }
						
            decreaseThreshold();
            break;
    }

    return beatDetected;
}

void decreaseThreshold()
{
  
    if (lastMaxValue > 0 && beatPeriod > 0) 
		{
        threshold -= lastMaxValue * (1 - BEATDETECTOR_THRESHOLD_FALLOFF_TARGET) /
                (beatPeriod / BEATDETECTOR_SAMPLES_PERIOD); 
    } 
		else 
		{
      
        threshold *= BEATDETECTOR_THRESHOLD_DECAY_FACTOR;
    }

    if (threshold < BEATDETECTOR_MIN_THRESHOLD) 
		{
        threshold = BEATDETECTOR_MIN_THRESHOLD;
    }
}

int min(int a ,int b)
{
  return (a>b)?b:a;
}

