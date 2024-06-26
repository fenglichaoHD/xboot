#ifndef __AUDIO_H__
#define __AUDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <xboot.h>

enum audio_format_t {
	AUDIO_FORMAT_S8		= 8,
	AUDIO_FORMAT_S16	= 16,
	AUDIO_FORMAT_S24	= 24,
	AUDIO_FORMAT_S32	= 32,
};

enum audio_rate_t {
	AUDIO_RATE_7350		= 7350,
	AUDIO_RATE_8000		= 8000,
	AUDIO_RATE_11025	= 11025,
	AUDIO_RATE_12000	= 12000,
	AUDIO_RATE_14700	= 14700,
	AUDIO_RATE_16000	= 16000,
	AUDIO_RATE_22050	= 22050,
	AUDIO_RATE_24000	= 24000,
	AUDIO_RATE_29400	= 29400,
	AUDIO_RATE_32000	= 32000,
	AUDIO_RATE_44100	= 44100,
	AUDIO_RATE_48000	= 48000,
	AUDIO_RATE_88200	= 88200,
	AUDIO_RATE_96000	= 96000,
	AUDIO_RATE_176400	= 176400,
	AUDIO_RATE_192000	= 192000,
};

struct audio_t
{
	/* The audio name */
	char * name;

	/* The sound pool */
	struct {
		struct task_t * playback;
		struct list_head list;
		spinlock_t lock;
	} soundpool;

	/* Audio playback start */
	void (*playback_start)(struct audio_t * audio, enum audio_rate_t rate, enum audio_format_t fmt, int ch);

	/* Audio playback write */
	int (*playback_write)(struct audio_t * audio, void * buf, int len);

	/* Audio playback stop */
	void (*playback_stop)(struct audio_t * audio);

	/* Audio capture start */
	void (*capture_start)(struct audio_t * audio, enum audio_rate_t rate, enum audio_format_t fmt, int ch);

	/* Audio capture read */
	int (*capture_read)(struct audio_t * audio, void * buf, int len);

	/* Audio capture stop */
	void (*capture_stop)(struct audio_t * audio);

	/* Ioctl interface */
	int (*ioctl)(struct audio_t * audio, const char * cmd, void * arg);

	/* Private data */
	void * priv;
};

static inline int audio_playback_is_support(struct audio_t * audio)
{
	if(audio && audio->playback_start && audio->playback_write && audio->playback_stop)
		return 1;
	return 0;
}

static inline int audio_capture_is_support(struct audio_t * audio)
{
	if(audio && audio->capture_start && audio->capture_read && audio->capture_stop)
		return 1;
	return 0;
}

static int audio_ioctl(struct audio_t * audio, const char * cmd, void * arg)
{
	if(audio && audio->ioctl)
		return audio->ioctl(audio, cmd, arg);
	return -1;
}

static inline int audio_get_playback_volume(struct audio_t * audio)
{
	int vol;

	if(audio_ioctl(audio, "audio-get-playback-volume", &vol) >= 0)
		return vol;
	return 0;
}

static inline void audio_set_playback_volume(struct audio_t * audio, int vol)
{
	audio_ioctl(audio, "audio-set-playback-volume", &vol);
}

static inline int audio_get_capture_volume(struct audio_t * audio)
{
	int vol;

	if(audio_ioctl(audio, "audio-get-capture-volume", &vol) >= 0)
		return vol;
	return 0;
}

static inline void audio_set_capture_volume(struct audio_t * audio, int vol)
{
	audio_ioctl(audio, "audio-set-capture-volume", &vol);
}

struct audio_t * search_audio(const char * name);
struct audio_t * search_first_audio(void);
struct device_t * register_audio(struct audio_t * audio, struct driver_t * drv);
void unregister_audio(struct audio_t * audio);

void audio_playback_start(struct audio_t * audio, enum audio_rate_t rate, enum audio_format_t fmt, int ch);
int audio_playback_write(struct audio_t * audio, void * buf, int len);
void audio_playback_stop(struct audio_t * audio);
void audio_capture_start(struct audio_t * audio, enum audio_rate_t rate, enum audio_format_t fmt, int ch);
int audio_capture_read(struct audio_t * audio, void * buf, int len);
void audio_capture_stop(struct audio_t * audio);
int audio_ioctl(struct audio_t * audio, const char * cmd, void * arg);

void audio_playback(struct audio_t * audio, struct sound_t * snd);

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_H__ */
