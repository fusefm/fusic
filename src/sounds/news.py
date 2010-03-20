#!/usr/bin/python
#
# news - This object represents a news cart type sound.
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

class news(cart):

	# Constructor
	def __init__(self, file_ID):
		"""Constructs a new news cart from the latest news recorded."""
		# TODO - Add MySQL instructions here to set member variables.
		cart.__init__(self, file_ID) # TODO - Ensure that the correct cart is
		# constructed here.

	# Class properties
	@property
	def date_time(self):
		"""Returns the date/time of the news recorded."""
		return 0

	@property
	def presenter(self):
		"""Returns the user_id of the presenter that recorded the news."""
		return 0
