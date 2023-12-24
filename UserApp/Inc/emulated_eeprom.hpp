/*
 * @file:emulated_eeprom.h
 * @brief:
 * @Date: 2023年12月24日
 * @Author: iTuring
 */

#ifndef INC_EMULATED_EEPROM_HPP_
#define INC_EMULATED_EEPROM_HPP_

#ifdef __cplusplus
 extern "C" {
#endif


#ifdef __cplusplus
}


 class EEPROMClass
 {
 public:
	 template <typename T>
	 inline T const& Max (T const& a, T const& b)
	 {
		 return a < b ? b:a;
	 }
 };

#endif
#endif /* INC_EMULATED_EEPROM_HPP_ */
