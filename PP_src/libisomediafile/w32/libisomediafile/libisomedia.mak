# Microsoft Developer Studio Generated NMAKE File, Based on libisomedia.dsp
!IF "$(CFG)" == ""
CFG=libisomedia - Win32 Debug
!MESSAGE No configuration specified. Defaulting to libisomedia - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libisomedia - Win32 Release" && "$(CFG)" != "libisomedia - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libisomedia.mak" CFG="libisomedia - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libisomedia - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libisomedia - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libisomedia - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release\obj
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\libisomedia.dll" "$(OUTDIR)\libisomedia.bsc"


CLEAN :
	-@erase "$(INTDIR)\AudioSampleEntryAtom.obj"
	-@erase "$(INTDIR)\AudioSampleEntryAtom.sbr"
	-@erase "$(INTDIR)\BaseDescriptor.obj"
	-@erase "$(INTDIR)\BaseDescriptor.sbr"
	-@erase "$(INTDIR)\ChunkLargeOffsetAtom.obj"
	-@erase "$(INTDIR)\ChunkLargeOffsetAtom.sbr"
	-@erase "$(INTDIR)\ChunkOffsetAtom.obj"
	-@erase "$(INTDIR)\ChunkOffsetAtom.sbr"
	-@erase "$(INTDIR)\ClockReferenceMediaHeader.obj"
	-@erase "$(INTDIR)\ClockReferenceMediaHeader.sbr"
	-@erase "$(INTDIR)\CopyrightAtom.obj"
	-@erase "$(INTDIR)\CopyrightAtom.sbr"
	-@erase "$(INTDIR)\DataEntryURLAtom.obj"
	-@erase "$(INTDIR)\DataEntryURLAtom.sbr"
	-@erase "$(INTDIR)\DataEntryURNAtom.obj"
	-@erase "$(INTDIR)\DataEntryURNAtom.sbr"
	-@erase "$(INTDIR)\DataInformationAtom.obj"
	-@erase "$(INTDIR)\DataInformationAtom.sbr"
	-@erase "$(INTDIR)\DataReferenceAtom.obj"
	-@erase "$(INTDIR)\DataReferenceAtom.sbr"
	-@erase "$(INTDIR)\DecodingOffsetAtom.obj"
	-@erase "$(INTDIR)\DecodingOffsetAtom.sbr"
	-@erase "$(INTDIR)\DegradationPriorityAtom.obj"
	-@erase "$(INTDIR)\DegradationPriorityAtom.sbr"
	-@erase "$(INTDIR)\EditAtom.obj"
	-@erase "$(INTDIR)\EditAtom.sbr"
	-@erase "$(INTDIR)\EditListAtom.obj"
	-@erase "$(INTDIR)\EditListAtom.sbr"
	-@erase "$(INTDIR)\ESDAtom.obj"
	-@erase "$(INTDIR)\ESDAtom.sbr"
	-@erase "$(INTDIR)\ESUpdateCommand.obj"
	-@erase "$(INTDIR)\ESUpdateCommand.sbr"
	-@erase "$(INTDIR)\FileMappingDataHandler.obj"
	-@erase "$(INTDIR)\FileMappingDataHandler.sbr"
	-@erase "$(INTDIR)\FreeSpaceAtom.obj"
	-@erase "$(INTDIR)\FreeSpaceAtom.sbr"
	-@erase "$(INTDIR)\GenericSampleEntryAtom.obj"
	-@erase "$(INTDIR)\GenericSampleEntryAtom.sbr"
	-@erase "$(INTDIR)\HandlerAtom.obj"
	-@erase "$(INTDIR)\HandlerAtom.sbr"
	-@erase "$(INTDIR)\HintMediaHeaderAtom.obj"
	-@erase "$(INTDIR)\HintMediaHeaderAtom.sbr"
	-@erase "$(INTDIR)\ISOMovieFile.obj"
	-@erase "$(INTDIR)\ISOMovieFile.sbr"
	-@erase "$(INTDIR)\MdatDataHandler.obj"
	-@erase "$(INTDIR)\MdatDataHandler.sbr"
	-@erase "$(INTDIR)\MediaAtom.obj"
	-@erase "$(INTDIR)\MediaAtom.sbr"
	-@erase "$(INTDIR)\MediaDataAtom.obj"
	-@erase "$(INTDIR)\MediaDataAtom.sbr"
	-@erase "$(INTDIR)\MediaHeaderAtom.obj"
	-@erase "$(INTDIR)\MediaHeaderAtom.sbr"
	-@erase "$(INTDIR)\MediaInformationAtom.obj"
	-@erase "$(INTDIR)\MediaInformationAtom.sbr"
	-@erase "$(INTDIR)\MemoryFileMappingObject.obj"
	-@erase "$(INTDIR)\MemoryFileMappingObject.sbr"
	-@erase "$(INTDIR)\MJ2BitsPerComponentAtom.obj"
	-@erase "$(INTDIR)\MJ2BitsPerComponentAtom.sbr"
	-@erase "$(INTDIR)\MJ2ColorSpecificationAtom.obj"
	-@erase "$(INTDIR)\MJ2ColorSpecificationAtom.sbr"
	-@erase "$(INTDIR)\MJ2FileTypeAtom.obj"
	-@erase "$(INTDIR)\MJ2FileTypeAtom.sbr"
	-@erase "$(INTDIR)\MJ2HeaderAtom.obj"
	-@erase "$(INTDIR)\MJ2HeaderAtom.sbr"
	-@erase "$(INTDIR)\MJ2ImageHeaderAtom.obj"
	-@erase "$(INTDIR)\MJ2ImageHeaderAtom.sbr"
	-@erase "$(INTDIR)\MJ2MovieFile.obj"
	-@erase "$(INTDIR)\MJ2MovieFile.sbr"
	-@erase "$(INTDIR)\MJ2Movies.obj"
	-@erase "$(INTDIR)\MJ2Movies.sbr"
	-@erase "$(INTDIR)\MJ2SignatureAtom.obj"
	-@erase "$(INTDIR)\MJ2SignatureAtom.sbr"
	-@erase "$(INTDIR)\MovieAtom.obj"
	-@erase "$(INTDIR)\MovieAtom.sbr"
	-@erase "$(INTDIR)\MovieHeaderAtom.obj"
	-@erase "$(INTDIR)\MovieHeaderAtom.sbr"
	-@erase "$(INTDIR)\MovieTracks.obj"
	-@erase "$(INTDIR)\MovieTracks.sbr"
	-@erase "$(INTDIR)\MP4Atoms.obj"
	-@erase "$(INTDIR)\MP4Atoms.sbr"
	-@erase "$(INTDIR)\MP4Commands.obj"
	-@erase "$(INTDIR)\MP4Commands.sbr"
	-@erase "$(INTDIR)\MP4DataHandler.obj"
	-@erase "$(INTDIR)\MP4DataHandler.sbr"
	-@erase "$(INTDIR)\MP4DecoderConfigDescriptor.obj"
	-@erase "$(INTDIR)\MP4DecoderConfigDescriptor.sbr"
	-@erase "$(INTDIR)\MP4DefaultCommand.obj"
	-@erase "$(INTDIR)\MP4DefaultCommand.sbr"
	-@erase "$(INTDIR)\MP4DefaultDescriptor.obj"
	-@erase "$(INTDIR)\MP4DefaultDescriptor.sbr"
	-@erase "$(INTDIR)\MP4Descriptors.obj"
	-@erase "$(INTDIR)\MP4Descriptors.sbr"
	-@erase "$(INTDIR)\MP4ES_ID_IncDescriptor.obj"
	-@erase "$(INTDIR)\MP4ES_ID_IncDescriptor.sbr"
	-@erase "$(INTDIR)\MP4ES_ID_RefDescriptor.obj"
	-@erase "$(INTDIR)\MP4ES_ID_RefDescriptor.sbr"
	-@erase "$(INTDIR)\MP4ESDescriptor.obj"
	-@erase "$(INTDIR)\MP4ESDescriptor.sbr"
	-@erase "$(INTDIR)\MP4FileMappingInputStream.obj"
	-@erase "$(INTDIR)\MP4FileMappingInputStream.sbr"
	-@erase "$(INTDIR)\MP4Handle.obj"
	-@erase "$(INTDIR)\MP4Handle.sbr"
	-@erase "$(INTDIR)\MP4InitialObjectDescriptor.obj"
	-@erase "$(INTDIR)\MP4InitialObjectDescriptor.sbr"
	-@erase "$(INTDIR)\MP4InputStream.obj"
	-@erase "$(INTDIR)\MP4InputStream.sbr"
	-@erase "$(INTDIR)\MP4LinkedList.obj"
	-@erase "$(INTDIR)\MP4LinkedList.sbr"
	-@erase "$(INTDIR)\MP4Media.obj"
	-@erase "$(INTDIR)\MP4Media.sbr"
	-@erase "$(INTDIR)\MP4MemoryInputStream.obj"
	-@erase "$(INTDIR)\MP4MemoryInputStream.sbr"
	-@erase "$(INTDIR)\MP4MovieFile.obj"
	-@erase "$(INTDIR)\MP4MovieFile.sbr"
	-@erase "$(INTDIR)\MP4Movies.obj"
	-@erase "$(INTDIR)\MP4Movies.sbr"
	-@erase "$(INTDIR)\MP4ObjectDescriptor.obj"
	-@erase "$(INTDIR)\MP4ObjectDescriptor.sbr"
	-@erase "$(INTDIR)\MP4ODTrackReader.obj"
	-@erase "$(INTDIR)\MP4ODTrackReader.sbr"
	-@erase "$(INTDIR)\MP4OrdinaryTrackReader.obj"
	-@erase "$(INTDIR)\MP4OrdinaryTrackReader.sbr"
	-@erase "$(INTDIR)\MP4SLPacket.obj"
	-@erase "$(INTDIR)\MP4SLPacket.sbr"
	-@erase "$(INTDIR)\MP4TrackReader.obj"
	-@erase "$(INTDIR)\MP4TrackReader.sbr"
	-@erase "$(INTDIR)\MP4UserData.obj"
	-@erase "$(INTDIR)\MP4UserData.sbr"
	-@erase "$(INTDIR)\MPEGMediaHeaderAtom.obj"
	-@erase "$(INTDIR)\MPEGMediaHeaderAtom.sbr"
	-@erase "$(INTDIR)\MPEGSampleEntryAtom.obj"
	-@erase "$(INTDIR)\MPEGSampleEntryAtom.sbr"
	-@erase "$(INTDIR)\ObjectDescriptorAtom.obj"
	-@erase "$(INTDIR)\ObjectDescriptorAtom.sbr"
	-@erase "$(INTDIR)\ObjectDescriptorMediaHeader.obj"
	-@erase "$(INTDIR)\ObjectDescriptorMediaHeader.sbr"
	-@erase "$(INTDIR)\ODUpdateCommand.obj"
	-@erase "$(INTDIR)\ODUpdateCommand.sbr"
	-@erase "$(INTDIR)\QTMovieFile.obj"
	-@erase "$(INTDIR)\QTMovieFile.sbr"
	-@erase "$(INTDIR)\QTMovies.obj"
	-@erase "$(INTDIR)\QTMovies.sbr"
	-@erase "$(INTDIR)\SampleDescriptionAtom.obj"
	-@erase "$(INTDIR)\SampleDescriptionAtom.sbr"
	-@erase "$(INTDIR)\SampleSizeAtom.obj"
	-@erase "$(INTDIR)\SampleSizeAtom.sbr"
	-@erase "$(INTDIR)\SampleTableAtom.obj"
	-@erase "$(INTDIR)\SampleTableAtom.sbr"
	-@erase "$(INTDIR)\SampleToChunkAtom.obj"
	-@erase "$(INTDIR)\SampleToChunkAtom.sbr"
	-@erase "$(INTDIR)\SceneDescriptionMediaHeader.obj"
	-@erase "$(INTDIR)\SceneDescriptionMediaHeader.sbr"
	-@erase "$(INTDIR)\ShadowSyncAtom.obj"
	-@erase "$(INTDIR)\ShadowSyncAtom.sbr"
	-@erase "$(INTDIR)\SLConfigDescriptor.obj"
	-@erase "$(INTDIR)\SLConfigDescriptor.sbr"
	-@erase "$(INTDIR)\SoundMediaHeaderAtom.obj"
	-@erase "$(INTDIR)\SoundMediaHeaderAtom.sbr"
	-@erase "$(INTDIR)\SyncSampleAtom.obj"
	-@erase "$(INTDIR)\SyncSampleAtom.sbr"
	-@erase "$(INTDIR)\TimeToSampleAtom.obj"
	-@erase "$(INTDIR)\TimeToSampleAtom.sbr"
	-@erase "$(INTDIR)\TrackAtom.obj"
	-@erase "$(INTDIR)\TrackAtom.sbr"
	-@erase "$(INTDIR)\TrackHeaderAtom.obj"
	-@erase "$(INTDIR)\TrackHeaderAtom.sbr"
	-@erase "$(INTDIR)\TrackReferenceAtom.obj"
	-@erase "$(INTDIR)\TrackReferenceAtom.sbr"
	-@erase "$(INTDIR)\TrackReferenceTypeAtom.obj"
	-@erase "$(INTDIR)\TrackReferenceTypeAtom.sbr"
	-@erase "$(INTDIR)\UnknownAtom.obj"
	-@erase "$(INTDIR)\UnknownAtom.sbr"
	-@erase "$(INTDIR)\UserDataAtom.obj"
	-@erase "$(INTDIR)\UserDataAtom.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VideoMediaHeaderAtom.obj"
	-@erase "$(INTDIR)\VideoMediaHeaderAtom.sbr"
	-@erase "$(INTDIR)\VisualSampleEntryAtom.obj"
	-@erase "$(INTDIR)\VisualSampleEntryAtom.sbr"
	-@erase "$(INTDIR)\W32FileMappingObject.obj"
	-@erase "$(INTDIR)\W32FileMappingObject.sbr"
	-@erase "$(OUTDIR)\libisomedia.bsc"
	-@erase "$(OUTDIR)\libisomedia.dll"
	-@erase "$(OUTDIR)\libisomedia.exp"
	-@erase "$(OUTDIR)\libisomedia.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\w32" /I "..\..\w32" /I "..\..\src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "ISOMP4DLLAPI" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\libisomedia.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libisomedia.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AudioSampleEntryAtom.sbr" \
	"$(INTDIR)\BaseDescriptor.sbr" \
	"$(INTDIR)\ChunkLargeOffsetAtom.sbr" \
	"$(INTDIR)\ChunkOffsetAtom.sbr" \
	"$(INTDIR)\ClockReferenceMediaHeader.sbr" \
	"$(INTDIR)\CopyrightAtom.sbr" \
	"$(INTDIR)\DataEntryURLAtom.sbr" \
	"$(INTDIR)\DataEntryURNAtom.sbr" \
	"$(INTDIR)\DataInformationAtom.sbr" \
	"$(INTDIR)\DataReferenceAtom.sbr" \
	"$(INTDIR)\DecodingOffsetAtom.sbr" \
	"$(INTDIR)\DegradationPriorityAtom.sbr" \
	"$(INTDIR)\EditAtom.sbr" \
	"$(INTDIR)\EditListAtom.sbr" \
	"$(INTDIR)\ESDAtom.sbr" \
	"$(INTDIR)\ESUpdateCommand.sbr" \
	"$(INTDIR)\FileMappingDataHandler.sbr" \
	"$(INTDIR)\FreeSpaceAtom.sbr" \
	"$(INTDIR)\GenericSampleEntryAtom.sbr" \
	"$(INTDIR)\HandlerAtom.sbr" \
	"$(INTDIR)\HintMediaHeaderAtom.sbr" \
	"$(INTDIR)\MdatDataHandler.sbr" \
	"$(INTDIR)\MediaAtom.sbr" \
	"$(INTDIR)\MediaDataAtom.sbr" \
	"$(INTDIR)\MediaHeaderAtom.sbr" \
	"$(INTDIR)\MediaInformationAtom.sbr" \
	"$(INTDIR)\MemoryFileMappingObject.sbr" \
	"$(INTDIR)\MJ2BitsPerComponentAtom.sbr" \
	"$(INTDIR)\MJ2ColorSpecificationAtom.sbr" \
	"$(INTDIR)\MJ2FileTypeAtom.sbr" \
	"$(INTDIR)\MJ2HeaderAtom.sbr" \
	"$(INTDIR)\MJ2ImageHeaderAtom.sbr" \
	"$(INTDIR)\MJ2MovieFile.sbr" \
	"$(INTDIR)\MJ2Movies.sbr" \
	"$(INTDIR)\MJ2SignatureAtom.sbr" \
	"$(INTDIR)\MovieAtom.sbr" \
	"$(INTDIR)\MovieHeaderAtom.sbr" \
	"$(INTDIR)\MovieTracks.sbr" \
	"$(INTDIR)\MP4Atoms.sbr" \
	"$(INTDIR)\MP4Commands.sbr" \
	"$(INTDIR)\MP4DataHandler.sbr" \
	"$(INTDIR)\MP4DecoderConfigDescriptor.sbr" \
	"$(INTDIR)\MP4DefaultCommand.sbr" \
	"$(INTDIR)\MP4DefaultDescriptor.sbr" \
	"$(INTDIR)\MP4Descriptors.sbr" \
	"$(INTDIR)\MP4ES_ID_IncDescriptor.sbr" \
	"$(INTDIR)\MP4ES_ID_RefDescriptor.sbr" \
	"$(INTDIR)\MP4ESDescriptor.sbr" \
	"$(INTDIR)\MP4FileMappingInputStream.sbr" \
	"$(INTDIR)\MP4Handle.sbr" \
	"$(INTDIR)\MP4InitialObjectDescriptor.sbr" \
	"$(INTDIR)\MP4InputStream.sbr" \
	"$(INTDIR)\MP4LinkedList.sbr" \
	"$(INTDIR)\MP4Media.sbr" \
	"$(INTDIR)\MP4MemoryInputStream.sbr" \
	"$(INTDIR)\MP4MovieFile.sbr" \
	"$(INTDIR)\MP4Movies.sbr" \
	"$(INTDIR)\MP4ObjectDescriptor.sbr" \
	"$(INTDIR)\MP4ODTrackReader.sbr" \
	"$(INTDIR)\MP4OrdinaryTrackReader.sbr" \
	"$(INTDIR)\MP4SLPacket.sbr" \
	"$(INTDIR)\MP4TrackReader.sbr" \
	"$(INTDIR)\MP4UserData.sbr" \
	"$(INTDIR)\MPEGMediaHeaderAtom.sbr" \
	"$(INTDIR)\MPEGSampleEntryAtom.sbr" \
	"$(INTDIR)\ObjectDescriptorAtom.sbr" \
	"$(INTDIR)\ObjectDescriptorMediaHeader.sbr" \
	"$(INTDIR)\ODUpdateCommand.sbr" \
	"$(INTDIR)\SampleDescriptionAtom.sbr" \
	"$(INTDIR)\SampleSizeAtom.sbr" \
	"$(INTDIR)\SampleTableAtom.sbr" \
	"$(INTDIR)\SampleToChunkAtom.sbr" \
	"$(INTDIR)\SceneDescriptionMediaHeader.sbr" \
	"$(INTDIR)\ShadowSyncAtom.sbr" \
	"$(INTDIR)\SLConfigDescriptor.sbr" \
	"$(INTDIR)\SoundMediaHeaderAtom.sbr" \
	"$(INTDIR)\SyncSampleAtom.sbr" \
	"$(INTDIR)\TimeToSampleAtom.sbr" \
	"$(INTDIR)\TrackAtom.sbr" \
	"$(INTDIR)\TrackHeaderAtom.sbr" \
	"$(INTDIR)\TrackReferenceAtom.sbr" \
	"$(INTDIR)\TrackReferenceTypeAtom.sbr" \
	"$(INTDIR)\UnknownAtom.sbr" \
	"$(INTDIR)\UserDataAtom.sbr" \
	"$(INTDIR)\VideoMediaHeaderAtom.sbr" \
	"$(INTDIR)\VisualSampleEntryAtom.sbr" \
	"$(INTDIR)\W32FileMappingObject.sbr" \
	"$(INTDIR)\ISOMovieFile.sbr" \
	"$(INTDIR)\QTMovies.sbr" \
	"$(INTDIR)\QTMovieFile.sbr"

"$(OUTDIR)\libisomedia.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\libisomedia.pdb" /machine:I386 /out:"$(OUTDIR)\libisomedia.dll" /implib:"$(OUTDIR)\libisomedia.lib" 
LINK32_OBJS= \
	"$(INTDIR)\AudioSampleEntryAtom.obj" \
	"$(INTDIR)\BaseDescriptor.obj" \
	"$(INTDIR)\ChunkLargeOffsetAtom.obj" \
	"$(INTDIR)\ChunkOffsetAtom.obj" \
	"$(INTDIR)\ClockReferenceMediaHeader.obj" \
	"$(INTDIR)\CopyrightAtom.obj" \
	"$(INTDIR)\DataEntryURLAtom.obj" \
	"$(INTDIR)\DataEntryURNAtom.obj" \
	"$(INTDIR)\DataInformationAtom.obj" \
	"$(INTDIR)\DataReferenceAtom.obj" \
	"$(INTDIR)\DecodingOffsetAtom.obj" \
	"$(INTDIR)\DegradationPriorityAtom.obj" \
	"$(INTDIR)\EditAtom.obj" \
	"$(INTDIR)\EditListAtom.obj" \
	"$(INTDIR)\ESDAtom.obj" \
	"$(INTDIR)\ESUpdateCommand.obj" \
	"$(INTDIR)\FileMappingDataHandler.obj" \
	"$(INTDIR)\FreeSpaceAtom.obj" \
	"$(INTDIR)\GenericSampleEntryAtom.obj" \
	"$(INTDIR)\HandlerAtom.obj" \
	"$(INTDIR)\HintMediaHeaderAtom.obj" \
	"$(INTDIR)\MdatDataHandler.obj" \
	"$(INTDIR)\MediaAtom.obj" \
	"$(INTDIR)\MediaDataAtom.obj" \
	"$(INTDIR)\MediaHeaderAtom.obj" \
	"$(INTDIR)\MediaInformationAtom.obj" \
	"$(INTDIR)\MemoryFileMappingObject.obj" \
	"$(INTDIR)\MJ2BitsPerComponentAtom.obj" \
	"$(INTDIR)\MJ2ColorSpecificationAtom.obj" \
	"$(INTDIR)\MJ2FileTypeAtom.obj" \
	"$(INTDIR)\MJ2HeaderAtom.obj" \
	"$(INTDIR)\MJ2ImageHeaderAtom.obj" \
	"$(INTDIR)\MJ2MovieFile.obj" \
	"$(INTDIR)\MJ2Movies.obj" \
	"$(INTDIR)\MJ2SignatureAtom.obj" \
	"$(INTDIR)\MovieAtom.obj" \
	"$(INTDIR)\MovieHeaderAtom.obj" \
	"$(INTDIR)\MovieTracks.obj" \
	"$(INTDIR)\MP4Atoms.obj" \
	"$(INTDIR)\MP4Commands.obj" \
	"$(INTDIR)\MP4DataHandler.obj" \
	"$(INTDIR)\MP4DecoderConfigDescriptor.obj" \
	"$(INTDIR)\MP4DefaultCommand.obj" \
	"$(INTDIR)\MP4DefaultDescriptor.obj" \
	"$(INTDIR)\MP4Descriptors.obj" \
	"$(INTDIR)\MP4ES_ID_IncDescriptor.obj" \
	"$(INTDIR)\MP4ES_ID_RefDescriptor.obj" \
	"$(INTDIR)\MP4ESDescriptor.obj" \
	"$(INTDIR)\MP4FileMappingInputStream.obj" \
	"$(INTDIR)\MP4Handle.obj" \
	"$(INTDIR)\MP4InitialObjectDescriptor.obj" \
	"$(INTDIR)\MP4InputStream.obj" \
	"$(INTDIR)\MP4LinkedList.obj" \
	"$(INTDIR)\MP4Media.obj" \
	"$(INTDIR)\MP4MemoryInputStream.obj" \
	"$(INTDIR)\MP4MovieFile.obj" \
	"$(INTDIR)\MP4Movies.obj" \
	"$(INTDIR)\MP4ObjectDescriptor.obj" \
	"$(INTDIR)\MP4ODTrackReader.obj" \
	"$(INTDIR)\MP4OrdinaryTrackReader.obj" \
	"$(INTDIR)\MP4SLPacket.obj" \
	"$(INTDIR)\MP4TrackReader.obj" \
	"$(INTDIR)\MP4UserData.obj" \
	"$(INTDIR)\MPEGMediaHeaderAtom.obj" \
	"$(INTDIR)\MPEGSampleEntryAtom.obj" \
	"$(INTDIR)\ObjectDescriptorAtom.obj" \
	"$(INTDIR)\ObjectDescriptorMediaHeader.obj" \
	"$(INTDIR)\ODUpdateCommand.obj" \
	"$(INTDIR)\SampleDescriptionAtom.obj" \
	"$(INTDIR)\SampleSizeAtom.obj" \
	"$(INTDIR)\SampleTableAtom.obj" \
	"$(INTDIR)\SampleToChunkAtom.obj" \
	"$(INTDIR)\SceneDescriptionMediaHeader.obj" \
	"$(INTDIR)\ShadowSyncAtom.obj" \
	"$(INTDIR)\SLConfigDescriptor.obj" \
	"$(INTDIR)\SoundMediaHeaderAtom.obj" \
	"$(INTDIR)\SyncSampleAtom.obj" \
	"$(INTDIR)\TimeToSampleAtom.obj" \
	"$(INTDIR)\TrackAtom.obj" \
	"$(INTDIR)\TrackHeaderAtom.obj" \
	"$(INTDIR)\TrackReferenceAtom.obj" \
	"$(INTDIR)\TrackReferenceTypeAtom.obj" \
	"$(INTDIR)\UnknownAtom.obj" \
	"$(INTDIR)\UserDataAtom.obj" \
	"$(INTDIR)\VideoMediaHeaderAtom.obj" \
	"$(INTDIR)\VisualSampleEntryAtom.obj" \
	"$(INTDIR)\W32FileMappingObject.obj" \
	"$(INTDIR)\ISOMovieFile.obj" \
	"$(INTDIR)\QTMovies.obj" \
	"$(INTDIR)\QTMovieFile.obj"

"$(OUTDIR)\libisomedia.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug\obj
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\libisomedia.dll"


CLEAN :
	-@erase "$(INTDIR)\AudioSampleEntryAtom.obj"
	-@erase "$(INTDIR)\BaseDescriptor.obj"
	-@erase "$(INTDIR)\ChunkLargeOffsetAtom.obj"
	-@erase "$(INTDIR)\ChunkOffsetAtom.obj"
	-@erase "$(INTDIR)\ClockReferenceMediaHeader.obj"
	-@erase "$(INTDIR)\CopyrightAtom.obj"
	-@erase "$(INTDIR)\DataEntryURLAtom.obj"
	-@erase "$(INTDIR)\DataEntryURNAtom.obj"
	-@erase "$(INTDIR)\DataInformationAtom.obj"
	-@erase "$(INTDIR)\DataReferenceAtom.obj"
	-@erase "$(INTDIR)\DecodingOffsetAtom.obj"
	-@erase "$(INTDIR)\DegradationPriorityAtom.obj"
	-@erase "$(INTDIR)\EditAtom.obj"
	-@erase "$(INTDIR)\EditListAtom.obj"
	-@erase "$(INTDIR)\ESDAtom.obj"
	-@erase "$(INTDIR)\ESUpdateCommand.obj"
	-@erase "$(INTDIR)\FileMappingDataHandler.obj"
	-@erase "$(INTDIR)\FreeSpaceAtom.obj"
	-@erase "$(INTDIR)\GenericSampleEntryAtom.obj"
	-@erase "$(INTDIR)\HandlerAtom.obj"
	-@erase "$(INTDIR)\HintMediaHeaderAtom.obj"
	-@erase "$(INTDIR)\ISOMovieFile.obj"
	-@erase "$(INTDIR)\MdatDataHandler.obj"
	-@erase "$(INTDIR)\MediaAtom.obj"
	-@erase "$(INTDIR)\MediaDataAtom.obj"
	-@erase "$(INTDIR)\MediaHeaderAtom.obj"
	-@erase "$(INTDIR)\MediaInformationAtom.obj"
	-@erase "$(INTDIR)\MemoryFileMappingObject.obj"
	-@erase "$(INTDIR)\MJ2BitsPerComponentAtom.obj"
	-@erase "$(INTDIR)\MJ2ColorSpecificationAtom.obj"
	-@erase "$(INTDIR)\MJ2FileTypeAtom.obj"
	-@erase "$(INTDIR)\MJ2HeaderAtom.obj"
	-@erase "$(INTDIR)\MJ2ImageHeaderAtom.obj"
	-@erase "$(INTDIR)\MJ2MovieFile.obj"
	-@erase "$(INTDIR)\MJ2Movies.obj"
	-@erase "$(INTDIR)\MJ2SignatureAtom.obj"
	-@erase "$(INTDIR)\MovieAtom.obj"
	-@erase "$(INTDIR)\MovieHeaderAtom.obj"
	-@erase "$(INTDIR)\MovieTracks.obj"
	-@erase "$(INTDIR)\MP4Atoms.obj"
	-@erase "$(INTDIR)\MP4Commands.obj"
	-@erase "$(INTDIR)\MP4DataHandler.obj"
	-@erase "$(INTDIR)\MP4DecoderConfigDescriptor.obj"
	-@erase "$(INTDIR)\MP4DefaultCommand.obj"
	-@erase "$(INTDIR)\MP4DefaultDescriptor.obj"
	-@erase "$(INTDIR)\MP4Descriptors.obj"
	-@erase "$(INTDIR)\MP4ES_ID_IncDescriptor.obj"
	-@erase "$(INTDIR)\MP4ES_ID_RefDescriptor.obj"
	-@erase "$(INTDIR)\MP4ESDescriptor.obj"
	-@erase "$(INTDIR)\MP4FileMappingInputStream.obj"
	-@erase "$(INTDIR)\MP4Handle.obj"
	-@erase "$(INTDIR)\MP4InitialObjectDescriptor.obj"
	-@erase "$(INTDIR)\MP4InputStream.obj"
	-@erase "$(INTDIR)\MP4LinkedList.obj"
	-@erase "$(INTDIR)\MP4Media.obj"
	-@erase "$(INTDIR)\MP4MemoryInputStream.obj"
	-@erase "$(INTDIR)\MP4MovieFile.obj"
	-@erase "$(INTDIR)\MP4Movies.obj"
	-@erase "$(INTDIR)\MP4ObjectDescriptor.obj"
	-@erase "$(INTDIR)\MP4ODTrackReader.obj"
	-@erase "$(INTDIR)\MP4OrdinaryTrackReader.obj"
	-@erase "$(INTDIR)\MP4SLPacket.obj"
	-@erase "$(INTDIR)\MP4TrackReader.obj"
	-@erase "$(INTDIR)\MP4UserData.obj"
	-@erase "$(INTDIR)\MPEGMediaHeaderAtom.obj"
	-@erase "$(INTDIR)\MPEGSampleEntryAtom.obj"
	-@erase "$(INTDIR)\ObjectDescriptorAtom.obj"
	-@erase "$(INTDIR)\ObjectDescriptorMediaHeader.obj"
	-@erase "$(INTDIR)\ODUpdateCommand.obj"
	-@erase "$(INTDIR)\QTMovieFile.obj"
	-@erase "$(INTDIR)\QTMovies.obj"
	-@erase "$(INTDIR)\SampleDescriptionAtom.obj"
	-@erase "$(INTDIR)\SampleSizeAtom.obj"
	-@erase "$(INTDIR)\SampleTableAtom.obj"
	-@erase "$(INTDIR)\SampleToChunkAtom.obj"
	-@erase "$(INTDIR)\SceneDescriptionMediaHeader.obj"
	-@erase "$(INTDIR)\ShadowSyncAtom.obj"
	-@erase "$(INTDIR)\SLConfigDescriptor.obj"
	-@erase "$(INTDIR)\SoundMediaHeaderAtom.obj"
	-@erase "$(INTDIR)\SyncSampleAtom.obj"
	-@erase "$(INTDIR)\TimeToSampleAtom.obj"
	-@erase "$(INTDIR)\TrackAtom.obj"
	-@erase "$(INTDIR)\TrackHeaderAtom.obj"
	-@erase "$(INTDIR)\TrackReferenceAtom.obj"
	-@erase "$(INTDIR)\TrackReferenceTypeAtom.obj"
	-@erase "$(INTDIR)\UnknownAtom.obj"
	-@erase "$(INTDIR)\UserDataAtom.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VideoMediaHeaderAtom.obj"
	-@erase "$(INTDIR)\VisualSampleEntryAtom.obj"
	-@erase "$(INTDIR)\W32FileMappingObject.obj"
	-@erase "$(OUTDIR)\libisomedia.dll"
	-@erase "$(OUTDIR)\libisomedia.exp"
	-@erase "$(OUTDIR)\libisomedia.ilk"
	-@erase "$(OUTDIR)\libisomedia.lib"
	-@erase "$(OUTDIR)\libisomedia.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\w32" /I "..\..\src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "ISOMP4DLLAPI" /Fp"$(INTDIR)\libisomedia.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libisomedia.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\libisomedia.pdb" /debug /machine:I386 /out:"$(OUTDIR)\libisomedia.dll" /implib:"$(OUTDIR)\libisomedia.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AudioSampleEntryAtom.obj" \
	"$(INTDIR)\BaseDescriptor.obj" \
	"$(INTDIR)\ChunkLargeOffsetAtom.obj" \
	"$(INTDIR)\ChunkOffsetAtom.obj" \
	"$(INTDIR)\ClockReferenceMediaHeader.obj" \
	"$(INTDIR)\CopyrightAtom.obj" \
	"$(INTDIR)\DataEntryURLAtom.obj" \
	"$(INTDIR)\DataEntryURNAtom.obj" \
	"$(INTDIR)\DataInformationAtom.obj" \
	"$(INTDIR)\DataReferenceAtom.obj" \
	"$(INTDIR)\DecodingOffsetAtom.obj" \
	"$(INTDIR)\DegradationPriorityAtom.obj" \
	"$(INTDIR)\EditAtom.obj" \
	"$(INTDIR)\EditListAtom.obj" \
	"$(INTDIR)\ESDAtom.obj" \
	"$(INTDIR)\ESUpdateCommand.obj" \
	"$(INTDIR)\FileMappingDataHandler.obj" \
	"$(INTDIR)\FreeSpaceAtom.obj" \
	"$(INTDIR)\GenericSampleEntryAtom.obj" \
	"$(INTDIR)\HandlerAtom.obj" \
	"$(INTDIR)\HintMediaHeaderAtom.obj" \
	"$(INTDIR)\MdatDataHandler.obj" \
	"$(INTDIR)\MediaAtom.obj" \
	"$(INTDIR)\MediaDataAtom.obj" \
	"$(INTDIR)\MediaHeaderAtom.obj" \
	"$(INTDIR)\MediaInformationAtom.obj" \
	"$(INTDIR)\MemoryFileMappingObject.obj" \
	"$(INTDIR)\MJ2BitsPerComponentAtom.obj" \
	"$(INTDIR)\MJ2ColorSpecificationAtom.obj" \
	"$(INTDIR)\MJ2FileTypeAtom.obj" \
	"$(INTDIR)\MJ2HeaderAtom.obj" \
	"$(INTDIR)\MJ2ImageHeaderAtom.obj" \
	"$(INTDIR)\MJ2MovieFile.obj" \
	"$(INTDIR)\MJ2Movies.obj" \
	"$(INTDIR)\MJ2SignatureAtom.obj" \
	"$(INTDIR)\MovieAtom.obj" \
	"$(INTDIR)\MovieHeaderAtom.obj" \
	"$(INTDIR)\MovieTracks.obj" \
	"$(INTDIR)\MP4Atoms.obj" \
	"$(INTDIR)\MP4Commands.obj" \
	"$(INTDIR)\MP4DataHandler.obj" \
	"$(INTDIR)\MP4DecoderConfigDescriptor.obj" \
	"$(INTDIR)\MP4DefaultCommand.obj" \
	"$(INTDIR)\MP4DefaultDescriptor.obj" \
	"$(INTDIR)\MP4Descriptors.obj" \
	"$(INTDIR)\MP4ES_ID_IncDescriptor.obj" \
	"$(INTDIR)\MP4ES_ID_RefDescriptor.obj" \
	"$(INTDIR)\MP4ESDescriptor.obj" \
	"$(INTDIR)\MP4FileMappingInputStream.obj" \
	"$(INTDIR)\MP4Handle.obj" \
	"$(INTDIR)\MP4InitialObjectDescriptor.obj" \
	"$(INTDIR)\MP4InputStream.obj" \
	"$(INTDIR)\MP4LinkedList.obj" \
	"$(INTDIR)\MP4Media.obj" \
	"$(INTDIR)\MP4MemoryInputStream.obj" \
	"$(INTDIR)\MP4MovieFile.obj" \
	"$(INTDIR)\MP4Movies.obj" \
	"$(INTDIR)\MP4ObjectDescriptor.obj" \
	"$(INTDIR)\MP4ODTrackReader.obj" \
	"$(INTDIR)\MP4OrdinaryTrackReader.obj" \
	"$(INTDIR)\MP4SLPacket.obj" \
	"$(INTDIR)\MP4TrackReader.obj" \
	"$(INTDIR)\MP4UserData.obj" \
	"$(INTDIR)\MPEGMediaHeaderAtom.obj" \
	"$(INTDIR)\MPEGSampleEntryAtom.obj" \
	"$(INTDIR)\ObjectDescriptorAtom.obj" \
	"$(INTDIR)\ObjectDescriptorMediaHeader.obj" \
	"$(INTDIR)\ODUpdateCommand.obj" \
	"$(INTDIR)\SampleDescriptionAtom.obj" \
	"$(INTDIR)\SampleSizeAtom.obj" \
	"$(INTDIR)\SampleTableAtom.obj" \
	"$(INTDIR)\SampleToChunkAtom.obj" \
	"$(INTDIR)\SceneDescriptionMediaHeader.obj" \
	"$(INTDIR)\ShadowSyncAtom.obj" \
	"$(INTDIR)\SLConfigDescriptor.obj" \
	"$(INTDIR)\SoundMediaHeaderAtom.obj" \
	"$(INTDIR)\SyncSampleAtom.obj" \
	"$(INTDIR)\TimeToSampleAtom.obj" \
	"$(INTDIR)\TrackAtom.obj" \
	"$(INTDIR)\TrackHeaderAtom.obj" \
	"$(INTDIR)\TrackReferenceAtom.obj" \
	"$(INTDIR)\TrackReferenceTypeAtom.obj" \
	"$(INTDIR)\UnknownAtom.obj" \
	"$(INTDIR)\UserDataAtom.obj" \
	"$(INTDIR)\VideoMediaHeaderAtom.obj" \
	"$(INTDIR)\VisualSampleEntryAtom.obj" \
	"$(INTDIR)\W32FileMappingObject.obj" \
	"$(INTDIR)\ISOMovieFile.obj" \
	"$(INTDIR)\QTMovies.obj" \
	"$(INTDIR)\QTMovieFile.obj"

"$(OUTDIR)\libisomedia.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libisomedia.dep")
!INCLUDE "libisomedia.dep"
!ELSE 
!MESSAGE Warning: cannot find "libisomedia.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libisomedia - Win32 Release" || "$(CFG)" == "libisomedia - Win32 Debug"
SOURCE=..\..\src\AudioSampleEntryAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\AudioSampleEntryAtom.obj"	"$(INTDIR)\AudioSampleEntryAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\AudioSampleEntryAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\BaseDescriptor.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\BaseDescriptor.obj"	"$(INTDIR)\BaseDescriptor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\BaseDescriptor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ChunkLargeOffsetAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\ChunkLargeOffsetAtom.obj"	"$(INTDIR)\ChunkLargeOffsetAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\ChunkLargeOffsetAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ChunkOffsetAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\ChunkOffsetAtom.obj"	"$(INTDIR)\ChunkOffsetAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\ChunkOffsetAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ClockReferenceMediaHeader.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\ClockReferenceMediaHeader.obj"	"$(INTDIR)\ClockReferenceMediaHeader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\ClockReferenceMediaHeader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\CopyrightAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\CopyrightAtom.obj"	"$(INTDIR)\CopyrightAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\CopyrightAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\DataEntryURLAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\DataEntryURLAtom.obj"	"$(INTDIR)\DataEntryURLAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\DataEntryURLAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\DataEntryURNAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\DataEntryURNAtom.obj"	"$(INTDIR)\DataEntryURNAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\DataEntryURNAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\DataInformationAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\DataInformationAtom.obj"	"$(INTDIR)\DataInformationAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\DataInformationAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\DataReferenceAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\DataReferenceAtom.obj"	"$(INTDIR)\DataReferenceAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\DataReferenceAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\DecodingOffsetAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\DecodingOffsetAtom.obj"	"$(INTDIR)\DecodingOffsetAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\DecodingOffsetAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\DegradationPriorityAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\DegradationPriorityAtom.obj"	"$(INTDIR)\DegradationPriorityAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\DegradationPriorityAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\EditAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\EditAtom.obj"	"$(INTDIR)\EditAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\EditAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\EditListAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\EditListAtom.obj"	"$(INTDIR)\EditListAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\EditListAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ESDAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\ESDAtom.obj"	"$(INTDIR)\ESDAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\ESDAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ESUpdateCommand.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\ESUpdateCommand.obj"	"$(INTDIR)\ESUpdateCommand.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\ESUpdateCommand.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\FileMappingDataHandler.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\FileMappingDataHandler.obj"	"$(INTDIR)\FileMappingDataHandler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\FileMappingDataHandler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\FreeSpaceAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\FreeSpaceAtom.obj"	"$(INTDIR)\FreeSpaceAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\FreeSpaceAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\GenericSampleEntryAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\GenericSampleEntryAtom.obj"	"$(INTDIR)\GenericSampleEntryAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\GenericSampleEntryAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\HandlerAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\HandlerAtom.obj"	"$(INTDIR)\HandlerAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\HandlerAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\HintMediaHeaderAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\HintMediaHeaderAtom.obj"	"$(INTDIR)\HintMediaHeaderAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\HintMediaHeaderAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ISOMovieFile.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\ISOMovieFile.obj"	"$(INTDIR)\ISOMovieFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\ISOMovieFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MdatDataHandler.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MdatDataHandler.obj"	"$(INTDIR)\MdatDataHandler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MdatDataHandler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MediaAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MediaAtom.obj"	"$(INTDIR)\MediaAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MediaAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MediaDataAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MediaDataAtom.obj"	"$(INTDIR)\MediaDataAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MediaDataAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MediaHeaderAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MediaHeaderAtom.obj"	"$(INTDIR)\MediaHeaderAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MediaHeaderAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MediaInformationAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MediaInformationAtom.obj"	"$(INTDIR)\MediaInformationAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MediaInformationAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MemoryFileMappingObject.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MemoryFileMappingObject.obj"	"$(INTDIR)\MemoryFileMappingObject.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MemoryFileMappingObject.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MJ2BitsPerComponentAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MJ2BitsPerComponentAtom.obj"	"$(INTDIR)\MJ2BitsPerComponentAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MJ2BitsPerComponentAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MJ2ColorSpecificationAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MJ2ColorSpecificationAtom.obj"	"$(INTDIR)\MJ2ColorSpecificationAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MJ2ColorSpecificationAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MJ2FileTypeAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MJ2FileTypeAtom.obj"	"$(INTDIR)\MJ2FileTypeAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MJ2FileTypeAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MJ2HeaderAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MJ2HeaderAtom.obj"	"$(INTDIR)\MJ2HeaderAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MJ2HeaderAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MJ2ImageHeaderAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MJ2ImageHeaderAtom.obj"	"$(INTDIR)\MJ2ImageHeaderAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MJ2ImageHeaderAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MJ2MovieFile.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MJ2MovieFile.obj"	"$(INTDIR)\MJ2MovieFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MJ2MovieFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MJ2Movies.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MJ2Movies.obj"	"$(INTDIR)\MJ2Movies.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MJ2Movies.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MJ2SignatureAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MJ2SignatureAtom.obj"	"$(INTDIR)\MJ2SignatureAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MJ2SignatureAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MovieAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MovieAtom.obj"	"$(INTDIR)\MovieAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MovieAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MovieHeaderAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MovieHeaderAtom.obj"	"$(INTDIR)\MovieHeaderAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MovieHeaderAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MovieTracks.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MovieTracks.obj"	"$(INTDIR)\MovieTracks.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MovieTracks.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4Atoms.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4Atoms.obj"	"$(INTDIR)\MP4Atoms.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4Atoms.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4Commands.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4Commands.obj"	"$(INTDIR)\MP4Commands.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4Commands.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4DataHandler.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4DataHandler.obj"	"$(INTDIR)\MP4DataHandler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4DataHandler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4DecoderConfigDescriptor.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4DecoderConfigDescriptor.obj"	"$(INTDIR)\MP4DecoderConfigDescriptor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4DecoderConfigDescriptor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4DefaultCommand.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4DefaultCommand.obj"	"$(INTDIR)\MP4DefaultCommand.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4DefaultCommand.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4DefaultDescriptor.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4DefaultDescriptor.obj"	"$(INTDIR)\MP4DefaultDescriptor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4DefaultDescriptor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4Descriptors.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4Descriptors.obj"	"$(INTDIR)\MP4Descriptors.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4Descriptors.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4ES_ID_IncDescriptor.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4ES_ID_IncDescriptor.obj"	"$(INTDIR)\MP4ES_ID_IncDescriptor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4ES_ID_IncDescriptor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4ES_ID_RefDescriptor.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4ES_ID_RefDescriptor.obj"	"$(INTDIR)\MP4ES_ID_RefDescriptor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4ES_ID_RefDescriptor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4ESDescriptor.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4ESDescriptor.obj"	"$(INTDIR)\MP4ESDescriptor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4ESDescriptor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4FileMappingInputStream.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4FileMappingInputStream.obj"	"$(INTDIR)\MP4FileMappingInputStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4FileMappingInputStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4Handle.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4Handle.obj"	"$(INTDIR)\MP4Handle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4Handle.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4InitialObjectDescriptor.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4InitialObjectDescriptor.obj"	"$(INTDIR)\MP4InitialObjectDescriptor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4InitialObjectDescriptor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4InputStream.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4InputStream.obj"	"$(INTDIR)\MP4InputStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4InputStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4LinkedList.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4LinkedList.obj"	"$(INTDIR)\MP4LinkedList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4LinkedList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4Media.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4Media.obj"	"$(INTDIR)\MP4Media.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4Media.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4MemoryInputStream.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4MemoryInputStream.obj"	"$(INTDIR)\MP4MemoryInputStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4MemoryInputStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4MovieFile.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4MovieFile.obj"	"$(INTDIR)\MP4MovieFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4MovieFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4Movies.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4Movies.obj"	"$(INTDIR)\MP4Movies.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4Movies.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4ObjectDescriptor.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4ObjectDescriptor.obj"	"$(INTDIR)\MP4ObjectDescriptor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4ObjectDescriptor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4ODTrackReader.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4ODTrackReader.obj"	"$(INTDIR)\MP4ODTrackReader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4ODTrackReader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4OrdinaryTrackReader.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4OrdinaryTrackReader.obj"	"$(INTDIR)\MP4OrdinaryTrackReader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4OrdinaryTrackReader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4SLPacket.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4SLPacket.obj"	"$(INTDIR)\MP4SLPacket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4SLPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4TrackReader.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4TrackReader.obj"	"$(INTDIR)\MP4TrackReader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4TrackReader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MP4UserData.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MP4UserData.obj"	"$(INTDIR)\MP4UserData.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MP4UserData.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MPEGMediaHeaderAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MPEGMediaHeaderAtom.obj"	"$(INTDIR)\MPEGMediaHeaderAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MPEGMediaHeaderAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\MPEGSampleEntryAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\MPEGSampleEntryAtom.obj"	"$(INTDIR)\MPEGSampleEntryAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\MPEGSampleEntryAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ObjectDescriptorAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\ObjectDescriptorAtom.obj"	"$(INTDIR)\ObjectDescriptorAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\ObjectDescriptorAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ObjectDescriptorMediaHeader.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\ObjectDescriptorMediaHeader.obj"	"$(INTDIR)\ObjectDescriptorMediaHeader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\ObjectDescriptorMediaHeader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ODUpdateCommand.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\ODUpdateCommand.obj"	"$(INTDIR)\ODUpdateCommand.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\ODUpdateCommand.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\QTMovieFile.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\QTMovieFile.obj"	"$(INTDIR)\QTMovieFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\QTMovieFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\QTMovies.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\QTMovies.obj"	"$(INTDIR)\QTMovies.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\QTMovies.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\SampleDescriptionAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\SampleDescriptionAtom.obj"	"$(INTDIR)\SampleDescriptionAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\SampleDescriptionAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\SampleSizeAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\SampleSizeAtom.obj"	"$(INTDIR)\SampleSizeAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\SampleSizeAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\SampleTableAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\SampleTableAtom.obj"	"$(INTDIR)\SampleTableAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\SampleTableAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\SampleToChunkAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\SampleToChunkAtom.obj"	"$(INTDIR)\SampleToChunkAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\SampleToChunkAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\SceneDescriptionMediaHeader.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\SceneDescriptionMediaHeader.obj"	"$(INTDIR)\SceneDescriptionMediaHeader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\SceneDescriptionMediaHeader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ShadowSyncAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\ShadowSyncAtom.obj"	"$(INTDIR)\ShadowSyncAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\ShadowSyncAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\SLConfigDescriptor.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\SLConfigDescriptor.obj"	"$(INTDIR)\SLConfigDescriptor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\SLConfigDescriptor.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\SoundMediaHeaderAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\SoundMediaHeaderAtom.obj"	"$(INTDIR)\SoundMediaHeaderAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\SoundMediaHeaderAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\SyncSampleAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\SyncSampleAtom.obj"	"$(INTDIR)\SyncSampleAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\SyncSampleAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\TimeToSampleAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\TimeToSampleAtom.obj"	"$(INTDIR)\TimeToSampleAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\TimeToSampleAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\TrackAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\TrackAtom.obj"	"$(INTDIR)\TrackAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\TrackAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\TrackHeaderAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\TrackHeaderAtom.obj"	"$(INTDIR)\TrackHeaderAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\TrackHeaderAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\TrackReferenceAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\TrackReferenceAtom.obj"	"$(INTDIR)\TrackReferenceAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\TrackReferenceAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\TrackReferenceTypeAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\TrackReferenceTypeAtom.obj"	"$(INTDIR)\TrackReferenceTypeAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\TrackReferenceTypeAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\UnknownAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\UnknownAtom.obj"	"$(INTDIR)\UnknownAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\UnknownAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\UserDataAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\UserDataAtom.obj"	"$(INTDIR)\UserDataAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\UserDataAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\VideoMediaHeaderAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\VideoMediaHeaderAtom.obj"	"$(INTDIR)\VideoMediaHeaderAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\VideoMediaHeaderAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\VisualSampleEntryAtom.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\VisualSampleEntryAtom.obj"	"$(INTDIR)\VisualSampleEntryAtom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\VisualSampleEntryAtom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\W32FileMappingObject.c

!IF  "$(CFG)" == "libisomedia - Win32 Release"


"$(INTDIR)\W32FileMappingObject.obj"	"$(INTDIR)\W32FileMappingObject.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "libisomedia - Win32 Debug"


"$(INTDIR)\W32FileMappingObject.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

