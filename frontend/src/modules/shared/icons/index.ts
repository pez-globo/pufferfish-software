/**
 * @summary All icons (SVG) which are used in our App
 *
 * To convert existing SVG icon to be compatible to our App,
 * SVG tag should be replaced with `SvgIcon` tag of Material
 * And also copy the props to override settings using `<SvgIcon {...props}` /> syntac
 *
 * Example to use Icons in react compoennt
 *
 * <ClockIcon/>
 *
 * To override the settings for icon, use following syntax
 *
 * <ClockIcon style={{ fontSize: '2.5rem' }} />
 *
 */
export { default as BackIcon } from './BackIcon';
export { default as ChartIcon } from './ChartIcon';
export { default as GearIcon } from './GearIcon';
export { default as LungsIcon } from './LungsIcon';
export { default as TrendDownIcon } from './TrendDownIcon';
export { default as TrendUpIcon } from './TrendUpIcon';
