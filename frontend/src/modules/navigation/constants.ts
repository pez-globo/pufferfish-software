import quickStart from '../icons/quickStart.svg'
import modes from '../icons/modes.svg'
import setAlarms from '../icons/setAlarms.svg'
import values from '../icons/values.svg'
import settings from '../icons/settings.svg'

console.log(quickStart);

// Constants for storing objects of route paths. 
export const DASHBOARD_ROUTE = { label: 'Dashboard', path: '/dashboard' }
export const QUICKSTART_ROUTE = { icon: quickStart, label: 'Quick Start', path: '/quickstart' }
export const MODES_ROUTE = { icon: modes,label: 'Modes', path: '/modes' }
export const ALARMS_ROUTE = { icon: setAlarms,label: 'Set Alarms', path: '/alarms' }
export const LOGS_ROUTE = { label: 'Logs', path: '/logs' }
export const VALUES_ROUTE = { icon: values,label: 'All Values', path: '/values' }
export const SCREENSAVER_ROUTE = { label: 'Screensaver', path: '/screensaver' }
export const SETTINGS_ROUTE = { icon: settings,label: 'Settings', path: '/settings' }