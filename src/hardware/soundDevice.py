#!/usr/bin/python
#
# soundDevice - This is a high level sound hardware controller.
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

class soundDevice(object):

	# A list of currently loaded sound refernces, eg playing, paused and cued sounds.
	loaded_sounds = []

	def load(self, filename):
		""" 
		will load a sound from the given filename.
		Returns a playback object.
		"""
		pass

class playback(object):
	def _set_playing(self, value):
		pass
	def _get_playing(self):
		return false
	playing = property(
		fget = _get_playing,
		fset = _set_playing, 
		doc = "Changes the playing state of the object."
	)
	
	def _set_position(self, position):
		pass
	def _get_position(self):
		return 0
	position = property(
		fget = _get_position,
		fset = _set_position,
		doc = "The number of seconds into the file."
	)
	
	def _set_volume(self, volume):
		pass
	def _get_volume(self):
		return 1
	volume = property(
		fget = _get_volume,
		fset = _set_volume,
		doc = "The volume of playback. 1 - full volume. 0 - no volume."
	)
	
	@property
	def length(self):
		"""Will return the length of the file in seconds."""
		return 0
	
