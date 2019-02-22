#ifndef IN_H
#define IN_H

	void import_image(char* path); /*load an image*/
	short find_path(char* path);   /*return 1 if path is not found , else return 0 */
	short check_format(char* form_type);/*return 1 if the image format is not valid else return 0*/

#endif 