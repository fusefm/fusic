#!/usr/bin/python
#
# cart - This object represents a cart type sound.
# It will also hold data relevent to the posistion, colour and type of cart.
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

class cart(sound):

	# Constructor
	def __init__(self, cart_id, show_id):
		# TODO - Add MySQL instructions here to set member variables.
		sound.__init__(self, 0)# TODO - Ensure that the correct fileID is
		# passed to the sound constructor.

	# Class properties
	@property
	def posistion(self):
		return []

	@peoperty
	def colour(self):
		return 0

	@property
	def playmode(self):
		return 0