--
-- Database: `playoutsystem`
--

-- --------------------------------------------------------

--
-- Table structure for table `tbl_carts`
--

CREATE TABLE IF NOT EXISTS `tbl_carts` (
  `Cart_ID` varchar(3) NOT NULL,
  `Show_ID` int(11) NOT NULL,
  `File_ID` int(11) NOT NULL,
  `Cart_Colour` varchar(9) NOT NULL,
  `Cart_Title` varchar(100) NOT NULL,
  `Cart_Refresh` tinyint(1) NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Carts info';

-- --------------------------------------------------------

--
-- Table structure for table `tbl_files`
--

CREATE TABLE IF NOT EXISTS `tbl_files` (
  `File_ID` int(11) NOT NULL auto_increment,
  `File_Location` varchar(500) NOT NULL,
  `File_Type` char(1) NOT NULL,
  `File_Artist` varchar(50) NOT NULL,
  `File_Title` varchar(80) NOT NULL,
  `File_Album` varchar(50) NOT NULL,
  `File_Intro` double default NULL,
  `File_Duration` double NOT NULL,
  `File_Fadein` double default NULL,
  `File_Fadeout` double default NULL,
  `Show_ID` int(11) NOT NULL,
  `File_PlayedLast` datetime NOT NULL,
  `File_MusicBrainzArtist` varchar(200) NOT NULL,
  `File_MusicBrainzTrack` varchar(200) NOT NULL,
  PRIMARY KEY  (`File_ID`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='File Info' AUTO_INCREMENT=3920 ;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_logs`
--

CREATE TABLE IF NOT EXISTS `tbl_logs` (
  `Log_ID` int(11) NOT NULL auto_increment,
  `Log_Title` varchar(50) NOT NULL,
  `Log_Artist` varchar(50) NOT NULL,
  `Log_FileID` int(11) NOT NULL,
  `Log_Album` varchar(50) NOT NULL,
  `Log_Duration` double NOT NULL,
  `Log_ShowID` int(11) NOT NULL,
  `Log_TimePlayed` datetime NOT NULL,
  `Log_MusicBrainzArtist` varchar(200) NOT NULL,
  `Log_MusicBrainzTrack` varchar(200) NOT NULL,
  `Log_PlayedByShowID` int(11) NOT NULL,
  PRIMARY KEY  (`Log_ID`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='Logs of Files Played' AUTO_INCREMENT=11410 ;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_playlist`
--

CREATE TABLE IF NOT EXISTS `tbl_playlist` (
  `Playlist_ID` int(11) NOT NULL auto_increment,
  `Show_ID` int(11) NOT NULL,
  `Playlist_Name` varchar(50) NOT NULL,
  PRIMARY KEY  (`Playlist_ID`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='Playlist Info' AUTO_INCREMENT=48 ;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_playlist_files`
--

CREATE TABLE IF NOT EXISTS `tbl_playlist_files` (
  `Playlist_ID` int(11) NOT NULL,
  `File_ID` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Linkage for files and plylists';

-- --------------------------------------------------------

--
-- Table structure for table `tbl_settings`
--

CREATE TABLE IF NOT EXISTS `tbl_settings` (
  `Setting_Name` varchar(100) NOT NULL,
  `Setting_Value` varchar(500) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_show`
--

CREATE TABLE IF NOT EXISTS `tbl_show` (
  `Show_ID` int(11) NOT NULL auto_increment,
  `Show_Name` varchar(100) NOT NULL,
  PRIMARY KEY  (`Show_ID`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='Show info' AUTO_INCREMENT=367 ;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_show_users`
--

CREATE TABLE IF NOT EXISTS `tbl_show_users` (
  `Show_ID` int(11) NOT NULL,
  `User_ID` int(11) NOT NULL,
  PRIMARY KEY  (`Show_ID`,`User_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Linkage for Users and Shows';
