/*
	Copyright 2011 Niels Brouwers

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.#include <string.h>
*/

#ifndef __usb__
#define __usb__

#include <stdint.h>
#include <stdbool.h>
// Device descriptor.


typedef struct  {

        union {

                struct {
                        uint16_t bmStatus; // port status bits
                        uint16_t bmChange; // port status change bits
                } __attribute__((packed));
                uint32_t bmEvent;
                uint8_t evtBuff[4];
        };
} usb_hubEvent;
typedef struct
{
	uint8_t bLength;				// Length of this descriptor.
	uint8_t bDescriptorType;		// DEVICE descriptor type (USB_DESCRIPTOR_DEVICE).
	uint16_t bcdUSB;				// USB Spec Release Number (BCD).
	uint8_t bDeviceClass;			// Class code (assigned by the USB-IF). 0xFF-Vendor specific.
	uint8_t bDeviceSubClass;		// Subclass code (assigned by the USB-IF).
	uint8_t bDeviceProtocol;		// Protocol code (assigned by the USB-IF). 0xFF-Vendor specific.
	uint8_t bMaxPacketSize0;		// Maximum packet size for endpoint 0.
	uint16_t idVendor;				// Vendor ID (assigned by the USB-IF).
	uint16_t idProduct;				// Product ID (assigned by the manufacturer).
	uint16_t bcdDevice;				// Device release number (BCD).
	uint8_t iManufacturer;			// Index of String Descriptor describing the manufacturer.
	uint8_t iProduct;				// Index of String Descriptor describing the product.
	uint8_t iSerialNumber;			// Index of String Descriptor with the device's serial number.
	uint8_t bNumConfigurations;		// Number of possible configurations.
} usb_deviceDescriptor;

typedef struct  {
        uint8_t bDescLength; // descriptor length
        uint8_t bDescriptorType; // descriptor type
        uint8_t bNbrPorts; // number of ports a hub equiped with

        struct {
                uint16_t LogPwrSwitchMode : 2;
                uint16_t CompoundDevice : 1;
                uint16_t OverCurrentProtectMode : 2;
                uint16_t TTThinkTime : 2;
                uint16_t PortIndicatorsSupported : 1;
                uint16_t Reserved : 8;
        } __attribute__((packed));

        uint8_t bPwrOn2PwrGood;
        uint8_t bHubContrCurrent;
        uint8_t pad;
} usb_hubDescriptor;


// Configuration descriptor.
typedef struct
{
	uint8_t bLength;				// Length of this descriptor.
	uint8_t bDescriptorType;		// CONFIGURATION descriptor type (USB_DESCRIPTOR_CONFIGURATION).
	uint16_t wTotalLength;			// Total length of all descriptors for this configuration.
	uint8_t bNumInterfaces;			// Number of interfaces in this configuration.
	uint8_t bConfigurationValue;	// Value of this configuration (1 based).
	uint8_t iConfiguration;			// Index of String Descriptor describing the configuration.
	uint8_t bmAttributes;			// Configuration characteristics.
	uint8_t bMaxPower;				// Maximum power consumed by this configuration.
} usb_configurationDescriptor;

// Interface descriptor.
typedef struct
{
	uint8_t bLength;				// Length of this descriptor.
	uint8_t bDescriptorType;		// INTERFACE descriptor type (USB_DESCRIPTOR_INTERFACE).
	uint8_t bInterfaceNumber;		// Number of this interface (0 based).
	uint8_t bAlternateSetting;		// Value of this alternate interface setting.
	uint8_t bNumEndpoints;			// Number of endpoints in this interface.
	uint8_t bInterfaceClass;		// Class code (assigned by the USB-IF).  0xFF-Vendor specific.
	uint8_t bInterfaceSubClass;		// Subclass code (assigned by the USB-IF).
	uint8_t bInterfaceProtocol;		// Protocol code (assigned by the USB-IF).  0xFF-Vendor specific.
	uint8_t iInterface;				// Index of String Descriptor describing the interface.
} usb_interfaceDescriptor;

/* Endpoint descriptor structure */
typedef struct
{
	uint8_t bLength;				// Length of this descriptor.
	uint8_t bDescriptorType;		// ENDPOINT descriptor type (USB_DESCRIPTOR_ENDPOINT).
	uint8_t bEndpointAddress;		// Endpoint address. Bit 7 indicates direction (0=OUT, 1=IN).
	uint8_t bmAttributes;			// Endpoint transfer type.
	uint16_t wMaxPacketSize;		// Maximum packet size.
	uint8_t bInterval;				// Polling interval in frames.
} usb_endpointDescriptor;

// USB Setup Packet.
typedef struct
{
	uint8_t bmRequestType;			// 0 Bit-map of request type
	uint8_t bRequest;				// 1 Request
	uint16_t wValue;				// 2 Depends on bRequest
	uint16_t wIndex;				// 4 Depends on bRequest
	uint16_t wLength;				// 6 Depends on bRequest
} usb_setupPacket;

/**
 * USB endpoint.
 */
typedef struct
{
    // Endpoint address. Bit 7 indicates direction (out=0, in=1).
	uint8_t address;

	// Endpoint transfer type.
	uint8_t attributes;

	// Maximum packet size.
    uint16_t maxPacketSize;

    // The max3421e uses these bits to toggle between DATA0 and DATA1.
    uint8_t sendToggle;
    uint8_t receiveToggle;

} usb_endpoint;

/**
 * USB device.
 */
typedef struct
{
	// Device address.
	uint8_t address;

	// Indicates whether this device is active.
	uint8_t active;
  uint8_t port;
	// Endpoints.
	usb_endpoint control;
	usb_endpoint bulk_in, bulk_out;

	// First supported language (for retrieving Strings)
	uint16_t firstStringLanguage;

} usb_device;

typedef enum
{
	USB_CONNECT,
	USB_DISCONNECT,
	USB_ADRESSING_ERROR
} usb_eventType;

/* Common setup data constant combinations  */
#define bmREQ_GET_DESCR     USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_STANDARD|USB_SETUP_RECIPIENT_DEVICE     //get descriptor request type
#define bmREQ_SET           USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_STANDARD|USB_SETUP_RECIPIENT_DEVICE     //set request type for all but 'set feature' and 'set interface'
#define bmREQ_CL_GET_INTF   USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE     //get interface request type
#define bmREQ_CLEAR_PORT_FEATURE                USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_OTHER
#define bmREQ_SET_PORT_FEATURE                  USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_OTHER
#define bmREQ_GET_HUB_DESCR                    USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_DEVICE
#define bmHUB_PORT_STATUS_C_PORT_ENABLE         0x0002
#define bmHUB_PORT_STATUS_C_PORT_CONNECTION	0x0001
#define bmHUB_PORT_EVENT_CONNECT                (((0UL | bmHUB_PORT_STATUS_C_PORT_CONNECTION) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_CONNECTION)
#define bmHUB_PORT_EVENT_DISCONNECT             (((0UL | bmHUB_PORT_STATUS_C_PORT_CONNECTION) << 16) | bmHUB_PORT_STATUS_PORT_POWER)
#define bmHUB_PORT_EVENT_RESET_COMPLETE         (((0UL | bmHUB_PORT_STATUS_C_PORT_RESET) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_ENABLE | bmHUB_PORT_STATUS_PORT_CONNECTION)
#define bmHUB_PORT_STATUS_C_PORT_RESET          0x0010
#define bmHUB_PORT_EVENT_LS_CONNECT             (((0UL | bmHUB_PORT_STATUS_C_PORT_CONNECTION) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_CONNECTION | bmHUB_PORT_STATUS_PORT_LOW_SPEED)
#define bmHUB_PORT_EVENT_LS_RESET_COMPLETE      (((0UL | bmHUB_PORT_STATUS_C_PORT_RESET) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_ENABLE | bmHUB_PORT_STATUS_PORT_CONNECTION | bmHUB_PORT_STATUS_PORT_LOW_SPEED)
#define bmHUB_PORT_EVENT_LS_PORT_ENABLED        (((0UL | bmHUB_PORT_STATUS_C_PORT_CONNECTION | bmHUB_PORT_STATUS_C_PORT_ENABLE) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_ENABLE | bmHUB_PORT_STATUS_PORT_CONNECTION | bmHUB_PORT_STATUS_PORT_LOW_SPEED)
#define bmREQ_GET_PORT_STATUS                   USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_OTHER
#define bmHUB_PORT_STATUS_PORT_POWER		0x0100
#define bmHUB_PORT_STATUS_PORT_ENABLE           0x0002
#define bmHUB_PORT_STATUS_PORT_CONNECTION	0x0001
#define bmHUB_PORT_STATUS_PORT_LOW_SPEED	0x0200
#define bmHUB_PORT_EVENT_LS_PORT_ENABLED        (((0UL | bmHUB_PORT_STATUS_C_PORT_CONNECTION | bmHUB_PORT_STATUS_C_PORT_ENABLE) << 16) | bmHUB_PORT_STATUS_PORT_POWER | bmHUB_PORT_STATUS_PORT_ENABLE | bmHUB_PORT_STATUS_PORT_CONNECTION | bmHUB_PORT_STATUS_PORT_LOW_SPEED)
#define rMODE			0xd8
/* HID requests */
/*
#define bmREQ_HIDOUT        USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE
#define bmREQ_HIDIN         USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE
#define bmREQ_HIDREPORT     USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_STANDARD|USB_SETUP_RECIPIENT_INTERFACE
*/

#define USB_XFER_TIMEOUT    5000    // USB transfer timeout in milliseconds, per section 9.2.6.1 of USB 2.0 spec
#define USB_NAK_LIMIT       32000   // NAK limit for a transfer. o meand NAKs are not counted
#define USB_RETRY_LIMIT     3       // retry limit for a transfer
#define USB_SETTLE_DELAY    200     // settle delay in milliseconds
#define USB_NAK_NOWAIT      1       // used in Richard's PS2/Wiimote code

#define USB_NUMDEVICES  4           // Number of USB devices

/* USB state machine states */

#define USB_STATE_MASK                                      0xf0
#define USB_STATE_DETACHED                                  0x10
#define USB_DETACHED_SUBSTATE_INITIALIZE                    0x11
#define USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE               0x12
#define USB_DETACHED_SUBSTATE_ILLEGAL                       0x13
#define USB_ATTACHED_SUBSTATE_SETTLE                        0x20
#define USB_ATTACHED_SUBSTATE_RESET_DEVICE                  0x30
#define USB_ATTACHED_SUBSTATE_WAIT_RESET_COMPLETE           0x40
#define USB_ATTACHED_SUBSTATE_WAIT_SOF                      0x50
#define USB_ATTACHED_SUBSTATE_GET_DEVICE_DESCRIPTOR_SIZE    0x60
#define USB_STATE_ADDRESSING                                0x70
#define USB_STATE_CONFIGURING                               0x80
#define USB_STATE_RUNNING                                   0x90
#define USB_STATE_ERROR                                     0xa0
#define HUB_FEATURE_C_HUB_LOCAL_POWER           0
#define HUB_FEATURE_C_HUB_OVER_CURRENT          1
#define HUB_FEATURE_PORT_CONNECTION             0
#define HUB_FEATURE_PORT_ENABLE                 1
#define HUB_FEATURE_PORT_SUSPEND                2
#define HUB_FEATURE_PORT_OVER_CURRENT           3
#define HUB_FEATURE_PORT_RESET                  4
#define HUB_FEATURE_PORT_POWER                  8
#define HUB_FEATURE_PORT_LOW_SPEED              9
#define HUB_FEATURE_C_PORT_CONNECTION           16
#define HUB_FEATURE_C_PORT_ENABLE               17
#define HUB_FEATURE_C_PORT_SUSPEND              18
#define HUB_FEATURE_C_PORT_OVER_CURRENT         19
#define HUB_FEATURE_C_PORT_RESET                20
#define HUB_FEATURE_PORT_TEST                   21
#define HUB_FEATURE_PORT_INDICATOR              22
//
// USB Device
typedef struct _usb_deviceRecord
{
    usb_endpoint * epinfo;      //device endpoint information
    uint8_t devclass;          //device class
} usb_deviceRecord;

typedef void(usb_eventHandler)(usb_device * device, usb_eventType event);

class USB
{

private:
	static void fireEvent(usb_device * device, usb_eventType event);
	static int setAddress(usb_device * device, uint8_t address);
	static int controlRequest(usb_device * device, uint8_t requestType, uint8_t request, uint8_t valueLow, uint8_t valueHigh, uint16_t index, uint16_t length, uint8_t * data);
	static int write(usb_device * device, usb_endpoint * endpoint, uint16_t length, uint8_t * data);
	static uint8_t ctrlData(usb_device * device, boolean direction, uint16_t length, uint8_t * data);

public:
	static void init();
	static void poll();
	static void setEventHandler(usb_eventHandler * handler);
        static void newDevice(int port);
        static void removeDevice(int port);
	static int initDevice(usb_device * device, int configuration);
	static usb_device * getDevice(uint8_t address);

	static int setConfiguration(usb_device * device, uint8_t configuration);
        static int setPortFeature(usb_device *device,uint8_t fid, uint8_t port, uint8_t sel);
        static int clearPortFeature(usb_device *device,uint8_t fid, uint8_t port, uint8_t sel);
        static int getPortStatus(usb_device *device,uint8_t port,uint16_t nbytes, uint8_t*dataptr);
        static void resetHubPort(usb_device *device,uint8_t port);
	static int getHubDescriptor(usb_device * device, usb_hubDescriptor * descriptor);

	static int getDeviceDescriptor(usb_device * device, usb_deviceDescriptor * descriptor);
	static int getConfigurationDescriptor(usb_device * device, uint8_t conf, uint16_t length, uint8_t * data);
	static int getString(usb_device * device, uint8_t index, uint8_t languageId, uint16_t length, char * str);

	static void initEndPoint(usb_endpoint * endpoint, uint8_t address);
	static int bulkRead(usb_device * device, uint16_t length, uint8_t * data, boolean poll);
	static int bulkWrite(usb_device * device, uint16_t length, uint8_t * data);
	static int read(usb_device * device, usb_endpoint * endpoint, uint16_t length, uint8_t * data, unsigned int nakLimit);
        
};

#endif
