#!/usr/bin/python
#
# sound - This is a high level sound base class abstraction.
#
# This file is part of Fusic.
#
# Fusic is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Fusic is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Fusic.  If not, see <http://www.gnu.org/licenses/>.

class sound(object):

	# Constructor
	def __init__(self, fileID):
		"""
		Create a new sound with a given fileID (int).
		"""
		# TODO - Add MySQL instructions here to set member variables.
		pass

	# Class properties
	@property
	def location(self):
		"""Returns the file location."""
		return ""

	@property
	def title(self):
		"""Returns the file title."""
		return ""

	@property
	def duration(self):
		"""Returns the file duration."""
		return 0

	@property
	def last_played(self):
		"""Returns the time when the file was last played."""
		return 0