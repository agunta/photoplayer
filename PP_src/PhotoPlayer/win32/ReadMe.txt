PhotoPlayer MAF utils

ppmaf_build	- builds PP MAF MP4 file for a list of images.
ppmaf_play	- retrieves specific files according to metadata
		  (not implemented)
ppmaf_get	- extracts images from Photo Player MP4 file with
		  specified id or all files

Examples of usage:

ppmaf_build ppmaf.mp4 list1.txt list2.txt list3.txt

Note: the first parameter should be the output file name followed by
one or more files with image lists. The first line in a list file can
contain metadata for the collection separated by commas or semicolons.
Currently it can be category and author. If no metadata is available
for the collection the first line should be empty. The encoding of
metadata to BiM is not implemented - it is stored as simple text.
See the example lists:
list1.txt - category and author
list2.txt - category
list3.txt - no collection metadata

ppmaf_play ppmaf.mp4

This command will retrieve specific images according to metadata
(not implemented). Currently it prints information about the content
of PP MAF file.

ppmaf_get ppmaf.mp4
ppmaf_get ppmaf.mp4 3

The first command extracts all images from PP MAF file, the second
command extracts only the third image (referenced by third track in
MP4 file). The file names of the extracted images are created using
track numbers.
