#!/usr/bin/python
#
# song - This object represents a song type sound.
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

class song(sound):

	# Constructor
	def __init__(self):
		# TODO - Add MySQL instructions here to set member variables.
		sound.__init__(self, 0) # TODO - Ensure the correct file_id is
		# passed to the sound constructor.

	# Class properties
	@property
	def artist(self):
		return ""

	@property
	def genre(self):
		return ""

	@property
	def album(self):
		return ""

	@property
	def intro(self):
		return 0