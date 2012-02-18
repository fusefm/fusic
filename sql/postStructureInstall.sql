--
-- Post install script for the Fusic Database
--

-- Add the station's own show:

INSERT INTO tbl_show(Show_ID, Show_Name) VALUES (0,
  "Fuse FM (Default)"
  );


UPDATE tbl_show SET Show_ID = 0 WHERE Show_Name = 
  "Fuse FM (Default)";