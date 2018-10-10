enum SERVER_RESULT_MESSAGE_CODE{
	FILE_IS_EXISTED,
	FILE_IS_NOT_EXISTED,
	FILE_CAN_UPLOAD,
	RECEIVE_FILE_FAIL,
	WRITE_FILE_FAIL,
	FILE_TRANFER_INTERUPTED,
	INVALID_ARGUMENT

};
char *getErrorMessage();

void exit_error(int error_code);

void error(int error_code);