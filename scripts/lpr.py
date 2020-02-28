import sys
sys.path.append("../Service")
from LicensePlateIdentifier import LicensePlateIdentifier

if __name__ == "__main__":

    lpr = LicensePlateIdentifier(None, None)
    # print(lpr.get_plate_information("BD17ABC"))