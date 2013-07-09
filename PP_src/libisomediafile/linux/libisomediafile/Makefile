# $Id: Makefile,v 1.9 2001/06/06 11:06:57 rtm Exp $


VPATH = .. ../../src

CFLAGS = -I.. -I../../src -O -g -Wall

CC = gcc

sources = \
	AudioSampleEntryAtom.c \
	BaseDescriptor.c \
	ChunkLargeOffsetAtom.c \
	ChunkOffsetAtom.c \
	ClockReferenceMediaHeader.c \
	CopyrightAtom.c \
	DataEntryURLAtom.c \
	DataEntryURNAtom.c \
	DataInformationAtom.c \
	DataReferenceAtom.c \
	DecodingOffsetAtom.c \
	DegradationPriorityAtom.c \
	ESDAtom.c \
	ESUpdateCommand.c \
	EditAtom.c \
	EditListAtom.c \
	FileMappingDataHandler.c \
	FreeSpaceAtom.c \
	GenericSampleEntryAtom.c \
	HandlerAtom.c \
	HintMediaHeaderAtom.c \
	ISOMovieFile.c \
	MemoryFileMappingObject.c \
	MdatDataHandler.c \
	MJ2BitsPerComponentAtom.c \
	MJ2ColorSpecificationAtom.c \
	MJ2FileTypeAtom.c \
	MJ2HeaderAtom.c \
	MJ2ImageHeaderAtom.c \
	MJ2MovieFile.c \
	MJ2Movies.c \
	MJ2SignatureAtom.c \
	MP4Atoms.c \
	MP4Commands.c \
	MP4DataHandler.c \
	MP4DecoderConfigDescriptor.c \
	MP4DefaultCommand.c \
	MP4DefaultDescriptor.c \
	MP4Descriptors.c \
	MP4ESDescriptor.c \
	MP4ES_ID_IncDescriptor.c \
	MP4ES_ID_RefDescriptor.c \
	MP4FileMappingInputStream.c \
	MP4Handle.c \
	MP4InitialObjectDescriptor.c \
	MP4InputStream.c \
	MP4LinkedList.c \
	MP4Media.c \
	MP4MemoryInputStream.c \
	MP4MovieFile.c \
	MP4Movies.c \
	MP4ODTrackReader.c \
	MP4ObjectDescriptor.c \
	MP4OrdinaryTrackReader.c \
	MP4SLPacket.c \
	MP4TrackReader.c \
	MP4UserData.c \
	MPEGMediaHeaderAtom.c \
	MPEGSampleEntryAtom.c \
	MediaAtom.c \
	MediaDataAtom.c \
	MediaHeaderAtom.c \
	MediaInformationAtom.c \
	MovieAtom.c \
	MovieHeaderAtom.c \
	MovieTracks.c \
	ODUpdateCommand.c \
	ObjectDescriptorAtom.c \
	ObjectDescriptorMediaHeader.c \
	QTMovieFile.c \
	QTMovies.c \
	SLConfigDescriptor.c \
	SampleDescriptionAtom.c \
	SampleSizeAtom.c \
	SampleTableAtom.c \
	SampleToChunkAtom.c \
	SceneDescriptionMediaHeader.c \
	ShadowSyncAtom.c \
	SimpleFileMappingObject.c \
	SoundMediaHeaderAtom.c \
	SyncSampleAtom.c \
	TimeToSampleAtom.c \
	TrackAtom.c \
	TrackHeaderAtom.c \
	TrackReferenceAtom.c \
	TrackReferenceTypeAtom.c \
	UnknownAtom.c \
	UserDataAtom.c \
	VideoMediaHeaderAtom.c \
	VisualSampleEntryAtom.c

objects := $(patsubst %.c,%.o,$(sources))

libisomediafile.a : $(objects)
	ar r libisomediafile.a $(objects)
	-ranlib libisomediafile.a

clean:
	-rm $(objects) libisoInterface.so

%.d: %.c
	$(SHELL) -ec '$(CC) -M $(CFLAGS) $< | sed '\''s/$*.o/& $@/g'\'' > $@'

TAGS: $(sources)
	etags $^

#
# Include all dependency files
#
-include $(sources:.c=.d)
