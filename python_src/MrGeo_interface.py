from ctypes import cdll, c_double, c_void_p, c_size_t, POINTER, c_int

def Clib(path):

    lib = cdll.LoadLibrary(path)

    lib.QF_construct.argtypes = [c_size_t]
    lib.QF_construct.restype = c_void_p

    lib.AD_construct.argtypes = []
    lib.AD_construct.restype = c_void_p

    lib.AD_test_bases.argtypes = [c_void_p, c_void_p, c_size_t, c_size_t]
    lib.AD_test_bases.restype = None

    lib.AD_get_num_objects.argtypes = [c_void_p]
    lib.AD_get_num_objects.restype = c_size_t

    lib.AD_get_num_frames.argtypes = [c_void_p]
    lib.AD_get_num_frames.restype = c_size_t

    lib.AD_get_scales.argtypes = [c_void_p, c_size_t]
    lib.AD_get_scales.restype = POINTER(c_double * 3)

    lib.AD_get_locations.argtypes = [c_void_p, c_size_t]
    lib.AD_get_locations.restype = POINTER(c_double * 3)

    lib.AD_get_rotations.argtypes = [c_void_p, c_size_t]
    lib.AD_get_rotations.restype = POINTER(c_double * 4)

    def init(self, max_spin = 10):
        self.qf_obj = lib.QF_construct(max_spin)
        self.ad_obj = lib.AD_construct()

    def test_bases(self, num_lines = 1, num_frames = 1000):
        lib.AD_test_bases(self.ad_obj, self.qf_obj, num_lines, num_frames)

    def get_num_objects(self):
        return lib.AD_get_num_objects(self.ad_obj);

    def get_num_frames(self):
        return lib.AD_get_num_frames(self.ad_obj);

    def get_scales(self, i):
        return lib.AD_get_scales(self.ad_obj, i)

    def get_locations(self, i):
        return lib.AD_get_locations(self.ad_obj, i)

    def get_rotations(self, i):
        return lib.AD_get_rotations(self.ad_obj, i)

    return type("MrGeoLib", (), {\
        "__init__": init,\
        "test_bases": test_bases,\
        "get_num_objects": get_num_objects,\
        "get_num_frames": get_num_frames,\
        "get_scales": get_scales,\
        "get_locations": get_locations,\
        "get_rotations": get_rotations,\
    })
