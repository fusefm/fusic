-- Procedure logFile(FileID INT). This procedure will copy file information
-- from the tbl_files table into the tbl_logs table to enable logging.

-- Used By: Fusic Client.

DELIMITER $$

DROP PROCEDURE IF EXISTS `p_logFile` $$
CREATE DEFINER=`playoutsystem`@`%` PROCEDURE `p_logFile`(IN FileID INT, IN playedShowID INT)
BEGIN

  -- Declare local variables:
  DECLARE v_Artist VARCHAR(255) DEFAULT '';
  DECLARE v_Title VARCHAR(255) DEFAULT '';
  DECLARE v_ID INT DEFAULT -1;
  DECLARE v_Album VARCHAR(255) DEFAULT '';
  DECLARE v_Duration DOUBLE DEFAULT 0;
  DECLARE v_ShowID INT DEFAULT 0;
  DECLARE v_TimeStamp	DATETIME DEFAULT NOW();
  DECLARE v_mbArtist VARCHAR(255) DEFAULT '';
  DECLARE v_mbTitle VARCHAR(255) DEFAULT '';


  -- Get info
  DECLARE cur_File CURSOR FOR SELECT File_Artist,
                                     File_Title,
                                     File_Album,
                                     File_Duration,
                                     Show_ID,
                                     File_MusicBrainzArtist,
                                     File_MusicBrainzTrack
                                     FROM tbl_files WHERE File_ID = FileID;

  DECLARE CONTINUE HANDLER FOR NOT FOUND BEGIN END;


  -- Open the cursor:
  OPEN cur_File;

  -- Get the values:
  FETCH cur_File INTO v_Artist, v_Title, v_Album, v_Duration, v_ShowID, v_mbArtist, v_mbTitle;

  -- Insert into the log table::
  INSERT INTO tbl_logs VALUES (0, v_Title, v_Artist, FileID, v_Album,
                              v_Duration, v_ShowID, v_TimeStamp, v_mbArtist, v_mbTitle, playedShowID);

  -- Set the last plaied time on the file also:
  UPDATE tbl_files SET File_PlayedLast = NOW() WHERE File_ID = FileID;

  -- Close the cursor:
  CLOSE cur_File;

  -- Done.
END $$

DELIMITER ;